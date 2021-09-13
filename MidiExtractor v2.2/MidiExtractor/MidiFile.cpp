#include <stdio.h> // printf
#include <stdint.h>
#include <stdlib.h> // malloc, free
#include <stdarg.h>
#include <iostream>
#include <string>
#include "MidiFile.h"
#include "MidiHelperData.h"
#include <memory>
#include <sstream>
#include <iomanip>

//------------------------------------------------------------------------------------
//
CMidiFile::CMidiFile(char* cpaBuffer, int iaBufferSize)
{
	ucp_buffer = cpaBuffer;
	i_BufferSize = iaBufferSize;
	i_BufferCurPos = 0;
	eof = FALSE;
	i_MidiFleType = 0;
	i_TracksInFile = 0;
	ui_DivisionPerQuarter = 96;
	i_MaxTicPosInFile = 0;

	for (int iI = 0; iI < MAX_NUMBER_OF_TRACKS; iI++)
		clpp_Track[iI] = nullptr;
	for (int iI = 0; iI < MAX_NUMBER_OF_CHANNELS; iI++)
		bp_ChannelUsed[iI] = FALSE;

	CT_MEMSET(cp_HeaderBuffer, 0, MIDI_FILE_HEADER_SIZE);

	i_c = -1;
	running_status = -1;
	eot = FALSE;
}



//------------------------------------------------------------------------------------
//
CMidiFile::~CMidiFile()
{
	if (ucp_buffer)
		CT_FREE(ucp_buffer);
	ucp_buffer = nullptr;

	for (int iI = 0; iI < MAX_NUMBER_OF_TRACKS; iI++)
	{
		if (clpp_Track[iI])
			delete clpp_Track[iI];
		clpp_Track[iI] = nullptr;
	}

	cl_TempoMapEvent.clear(); // NO need to delete as this is just a reference for the track events.
}



//------------------------------------------------------------------------------------
//
int CMidiFile::fnReadBuffer(void* buf, int len)
{
	int num = len < i_BufferSize - i_BufferCurPos
		? len : i_BufferSize - i_BufferCurPos;

	if (num != len)
	{
		eof = TRUE;
	}

	if (num < 0)
	{
		num = 0;
	}

	/* Note: Read bytes, even if there aren't enough, but only increment
	 * trackpos if successful (emulates old behaviour of ct_midi_file_read)
	 */
	CT_MEMCPY(buf, ucp_buffer + i_BufferCurPos, num);
	i_BufferCurPos += num;

	return (num != len) ? CT_FAILED : CT_OK;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnSkipBuffer(int skip)
{
	int new_pos = i_BufferCurPos + skip;

	/* Mimic the behaviour of fseek: Error to seek past the start of file, but
	 * OK to seek past end (this just puts it into the EOF state). */
	if (new_pos < 0)
	{
		CT_LOG(CT_ERR, "Failed to seek position in file");
		return CT_FAILED;
	}

	/* Clear the EOF flag, even if moved past the end of the file (this is
	 * consistent with the behaviour of fseek). */
	eof = FALSE;
	i_BufferCurPos = new_pos;
	return CT_OK;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnIsEOT()
{
	return eot || (i_BufferCurPos >= i_BufferSize);
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnGetc()
{
	unsigned char c;

	if (i_c >= 0)
	{
		c = i_c;
		i_c = -1;
	}
	else
	{
		if (i_BufferCurPos >= i_BufferSize)
		{
			eof = TRUE;
			return CT_FAILED;
		}

		c = ucp_buffer[i_BufferCurPos++];
	}

	return (int)c;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnPutc(int c)
{
	i_c = c;
	return CT_OK;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnGetVariableLengthValue()
{
	uint32_t v = 0;
	int8_t byte = 0;
	do {
		byte = fnGetc();
		v <<= 7;
		v |= (byte & 0x7F);
	} while (byte & (1 << 7));
	return (int)v;
}




//------------------------------------------------------------------------------------
//
ct_EventBuffer CMidiFile::fnWriteVariableLengthValue(int iaVal)
{
	ct_EventBuffer array ;

	bool isFirst = true;
	for (int i = 3; i >= 0; i--) {
		int b = iaVal >> (7 * i);
		int8_t byte = (int8_t)b & 127;
		if (!isFirst || byte > 0 || i == 0) {
			isFirst = false;
			if (i > 0) {
				// set 8th bit
				byte |= 128;
			}
			array.push_back(byte);
		}
	}

	return array;
}









//------------------------------------------------------------------------------------
//
int CMidiFile::fnAddTrack(CMidiTrack* clpaTrack, int iaMaxTickPosForTrack)
{
	int iRetVal = CT_FAILED;

	// Find the first empty track.
	for (int i = 0; i < MAX_NUMBER_OF_TRACKS; i++)
	{
		if (NULL == clpp_Track[i])
		{
			clpp_Track[i] = clpaTrack;
			iRetVal = CT_OK;
			break;
		}
	}

	if (CT_OK == iRetVal)
	{
		// Now lets add this tracks channel to our master file.
		for (int iJ = 0; iJ < MAX_NUMBER_OF_CHANNELS; iJ++)
		{
			if (clpaTrack->fnIsChannelUsed(iJ))
				bp_ChannelUsed[iJ] = TRUE;
		}

		// Added by AmitRungta now if this has a Tempo Node then add this in our master list also.
		for (auto eventmapitem : *clpaTrack->fnGetEventMap())
		{
			auto clpBaseEvent = eventmapitem.second;
			if (MIDI_SET_TEMPO == clpBaseEvent->fnGetEventType())
			{
				CTempoChangeMidiEvent* clpTempoChange = dynamic_cast<CTempoChangeMidiEvent*> (clpBaseEvent);
				if (clpTempoChange)
					cl_TempoMapEvent.insert(ct_TempoMap_Pair(eventmapitem.first, clpTempoChange));
			}
		}

		if (i_MaxTicPosInFile < iaMaxTickPosForTrack)
			i_MaxTicPosInFile = iaMaxTickPosForTrack;
	}


	return iRetVal;
}




//------------------------------------------------------------------------------------
//
CBaseMidiEvent* CMidiFile::fnReadEvent(bool& barOK, CMidiTrack* clpaTrack)
{
	barOK = true;

	// Get the starting pos of the buffer.
	const int niEventStartPos = i_BufferCurPos;
	CBaseMidiEvent* clpEventToRet = nullptr;



	/* read the status byte */
	int status = fnGetc();

	if (status < 0)
	{
		CT_LOG(CT_ERR, "Unexpected end of file");
		barOK = false;
		return nullptr;
	}

	/* not a valid status byte: use the running status instead */
	if ((status & 0x80) == 0)
	{
		if ((running_status & 0x80) == 0)
		{
			CT_LOG(CT_ERR, "Undefined status and invalid running status");
			barOK = false;
			return nullptr;
		}

		fnPutc(status);
		status = running_status;
	}

	/* check what message we have */
	running_status = status;


	if (status == MIDI_SYSEX)    /* system exclusif */
	{
		/* read the length of the message */
		int iValLen = fnGetVariableLengthValue();
		if (CT_FAILED == iValLen)
		{
			CT_LOG(CT_ERR, "Unexpected end of file");
			barOK = false;
			return nullptr;
		}
		if (fnSkipBuffer(iValLen) != CT_OK)
		{
			CT_LOG(CT_ERR, "Unexpected end of file");
			barOK = false;
			return nullptr;
		}

		clpEventToRet = new CGenericMidiEvent((unsigned char*)&ucp_buffer[niEventStartPos], i_BufferCurPos - niEventStartPos, MIDI_SYSEX, 0, 0);
	}
	else if (status == MIDI_META_EVENT)      /* meta events */
	{
		/* get the type of the meta message */
		int type = fnGetc();
		if (CT_FAILED == type)
		{
			CT_LOG(CT_ERR, "Unexpected end of file");
			barOK = false;
			return nullptr;
		}

		/* read the length of the message */
		int iValLen = fnGetVariableLengthValue();
		if (CT_FAILED == iValLen)
		{
			CT_LOG(CT_ERR, "Unexpected end of file");
			barOK = false;
			return nullptr;
		}

		std::unique_ptr<unsigned char> dummy_buffer(new unsigned char[iValLen + 1]);
		unsigned char* ucpMetaData = dummy_buffer.get();
		if (iValLen > 0)
		{
			if (CT_OK != fnReadBuffer(ucpMetaData, iValLen))
			{
				CT_LOG(CT_ERR, "Unexpected end of file");
				barOK = false;
				return nullptr;
			}
		}
		ucpMetaData[iValLen] = 0;

		if (MIDI_TRACK_NAME == type)
		{
			clpaTrack->fnSetTrackName((char*)ucpMetaData);
		}
		else if (MIDI_EOT == type)
		{
			if (0 != iValLen)
			{
				CT_LOG(CT_ERR, "Invalid length for EndOfTrack event");
				barOK = false;
				return nullptr;
			}
			eot = TRUE;
		}
		else if (MIDI_SET_TEMPO == type)
		{
			if (3 != iValLen)
			{
				CT_LOG(CT_ERR, "Invalid length for EndOfTrack event");
				barOK = false;
				return nullptr;
			}
			int tempo = (ucpMetaData[0] << 16) + (ucpMetaData[1] << 8) + ucpMetaData[2];
			clpEventToRet = new CTempoChangeMidiEvent((unsigned char*)&ucp_buffer[niEventStartPos], i_BufferCurPos - niEventStartPos, MIDI_SET_TEMPO, tempo, 0);
		}
		else if (MIDI_SMPTE_OFFSET == type)
		{
			if (5 != iValLen)
			{
				CT_LOG(CT_ERR, "Invalid length for SMPTE Offset meta event");
				barOK = false;
				return nullptr;
			}
		}
		else if (MIDI_TIME_SIGNATURE == type)
		{
			if (4 != iValLen)
			{
				CT_LOG(CT_ERR, "Invalid length for TimeSignature meta event");
				barOK = false;
				return nullptr;
			}
		}
		else if (MIDI_KEY_SIGNATURE == type)
		{
			if (2 != iValLen)
			{
				CT_LOG(CT_ERR, "Invalid length for KeySignature meta event");
				barOK = false;
				return nullptr;
			}
		}
		// Other cases in whcih i do not need to do anything
		//case MIDI_COPYRIGHT:
		//case MIDI_INST_NAME:
		//case MIDI_LYRIC:
		//case MIDI_TEXT:
		//case MIDI_MARKER:
		//case MIDI_CUE_POINT:
		//case MIDI_SEQUENCER_EVENT:
		if (!clpEventToRet)
		{
			clpEventToRet = new CGenericMidiEvent((unsigned char*)&ucp_buffer[niEventStartPos], i_BufferCurPos - niEventStartPos, type, 0, 0);
		}
	}
	else     /* channel messages */
	{
		// We are here this means its a channel based event.
		int type = status & 0xf0;
		int channel = status & 0x0f;

		/* all channel message have at least 1 byte of associated data */
		int iParam1 = fnGetc();
		if (CT_FAILED == iParam1)
		{
			CT_LOG(CT_ERR, "Unexpected end of file");
			barOK = false;
			return nullptr;
		}
		int iParam2 = 0;


		switch (type)
		{
		case NOTE_ON:
		{
			iParam2 = fnGetc();
			if (CT_FAILED == iParam2)
			{
				CT_LOG(CT_ERR, "Unexpected end of file");
				barOK = false;
				return nullptr;
			}
		}
		break;

		case NOTE_OFF:
			iParam2 = fnGetc();
			if (CT_FAILED == iParam2)
			{
				CT_LOG(CT_ERR, "Unexpected end of file");
				barOK = false;
				return nullptr;
			}

			break;

		case KEY_PRESSURE:
			iParam2 = fnGetc();
			if (CT_FAILED == iParam2)
			{
				CT_LOG(CT_ERR, "Unexpected end of file");
				barOK = false;
				return nullptr;
			}

			break;

		case CONTROL_CHANGE:
			iParam2 = fnGetc();
			if (CT_FAILED == iParam2)
			{
				CT_LOG(CT_ERR, "Unexpected end of file");
				barOK = false;
				return nullptr;
			}

			break;

		case PROGRAM_CHANGE:
			break;

		case CHANNEL_PRESSURE:
			break;

		case PITCH_BEND:
			iParam2 = fnGetc();
			if (CT_FAILED == iParam2)
			{
				CT_LOG(CT_ERR, "Unexpected end of file");
				barOK = false;
				return nullptr;
			}

			iParam1 = ((iParam2 & 0x7f) << 7) | (iParam1 & 0x7f);
			iParam2 = 0;
			break;

		default:
			/* Can't possibly happen !? */
			CT_LOG(CT_ERR, "Unrecognized MIDI event");
			barOK = false;
			return nullptr;
		}

		clpEventToRet = new CChannelBasedMidiEvent((unsigned char*)&ucp_buffer[niEventStartPos], i_BufferCurPos - niEventStartPos, type, iParam1, iParam2 , channel);
	}

	return clpEventToRet;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnGetMillisecondFromTick(int iaTick)
{
	double dTimeInMS = 0.;

	CTempoChangeMidiEvent* clpPrevEvent = nullptr;
	for (auto clTempoEventItem : cl_TempoMapEvent)
	{
		auto clpTempoEvent = clTempoEventItem.second;
		if (!clpTempoEvent)
			continue;

		if (!clpPrevEvent || clpTempoEvent->fnGetEventTickPos() <= iaTick)
		{
			if (!clpPrevEvent) {
				// first event in the list at time 0
				dTimeInMS = 0;
			}
			else {
				// any event before the endTick
				dTimeInMS += clpPrevEvent->fnGetMilliSecPerTick(ui_DivisionPerQuarter) * ((double)clpTempoEvent->fnGetEventTickPos() - clpPrevEvent->fnGetEventTickPos());
			}
			clpPrevEvent = clpTempoEvent;
		}
		else {
			// Current tempo event is greater than our existing event.
			break;
		}
	}

	if (!clpPrevEvent)
		return  0;

	dTimeInMS += clpPrevEvent->fnGetMilliSecPerTick(ui_DivisionPerQuarter) * ((double)iaTick - clpPrevEvent->fnGetEventTickPos());
	return (int)dTimeInMS;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnGetTickFromMillisecond(int iaTimeMS)
{
	if (cl_TempoMapEvent.size() < 1)
	{
		_ASSERT(FALSE);
		return 0;
	}
	if (iaTimeMS < 1)
		return 0;

	double dPrevEventTime = 0.;
	double dNextEventTime = 0.;

	CTempoChangeMidiEvent* clpPrevEvent = nullptr;
	for (auto clTempoEventItemItr = cl_TempoMapEvent.begin() ; clTempoEventItemItr != cl_TempoMapEvent.end() ; clTempoEventItemItr++)
	{
		auto clpTempoEvent = clTempoEventItemItr->second;
		auto clNextEventItr = clTempoEventItemItr; clNextEventItr++;
		if (!clpTempoEvent)
			continue;

		clpPrevEvent = clpTempoEvent;
		dPrevEventTime = dNextEventTime;

		if (clNextEventItr == cl_TempoMapEvent.end())
			break;

		int iTicksDiff = clNextEventItr->second->fnGetEventTickPos() - clpTempoEvent->fnGetEventTickPos();
		dNextEventTime += iTicksDiff * clpTempoEvent->fnGetMilliSecPerTick(ui_DivisionPerQuarter);
		if (dNextEventTime > iaTimeMS)
			break;
	}

	if (!clpPrevEvent)
		return 0;


	int iResTick = (iaTimeMS - dPrevEventTime) / clpPrevEvent->fnGetMilliSecPerTick(ui_DivisionPerQuarter) + clpPrevEvent->fnGetEventTickPos();
	return iResTick;
}




//------------------------------------------------------------------------------------
//
std::string CMidiFile::fnGetTimeDecoratedStringFromTick(int iaTick)
{
	int iRemainingTime = fnGetMillisecondFromTick(iaTick);

	int iMs = iRemainingTime % 1000;
	iRemainingTime /= 1000;

	int iSec = iRemainingTime % 60;
	iRemainingTime /= 60;

	int iMin = iRemainingTime % 60;
	iRemainingTime /= 60;
	int iHr = iRemainingTime;


	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << iHr << ':' << std::setw(2) << iMin << ':' << std::setw(2) << iSec << '.' << std::setw(3) << iMs;

	std::string time_obs_def = ss.str();
	return time_obs_def;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnGetMillisecondFromTimeDecoratedString(std::string claSTime)
{
	if (claSTime.empty())
		return 0;

	int h = 0, m = 0, s = 0, ms = 0;
	sscanf_s(claSTime.c_str(), "%d:%d:%d.%d", &h, &m, &s, &ms);

	int iTotalTimeInMS = (((h * 3600) + (m * 60) + s) * 1000) + ms;
	return iTotalTimeInMS;
}




//------------------------------------------------------------------------------------
//
int CMidiFile::fnGetTickFromTimeDecoratedString(std::string claSTime)
{
	return fnGetTickFromMillisecond(fnGetMillisecondFromTimeDecoratedString(claSTime));
}


