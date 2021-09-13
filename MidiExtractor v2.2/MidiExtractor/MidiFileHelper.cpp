#include <iostream>
#include <fstream>
#include <string>
#include "MidiFileHelper.h"
#include<vector>
#include<memory>
#include <json.hpp>
#include <iomanip>


using json = nlohmann::json;
#define JSON_BPM_TAG_NAME "BPM"



//------------------------------------------------------------------------------------
//
CMidiFileHelper::CMidiFileHelper()
{
	clp_MidiFile = nullptr;
}



//------------------------------------------------------------------------------------
//
CMidiFileHelper::~CMidiFileHelper()
{
	if ( clp_MidiFile)
		delete clp_MidiFile;
	clp_MidiFile = nullptr;
}




//------------------------------------------------------------------------------------
//
std::string CMidiFileHelper::fnGetInstrumentName(int iaProgID)
{
	switch (iaProgID + 1) {
	case 1: {
		return "Acoustic Grand Piano";
	}
	case 2: {
		return "Bright Acoustic Piano";
	}
	case 3: {
		return "Electric Grand Piano";
	}
	case 4: {
		return "Honky-tonk Piano";
	}
	case 5: {
		return " Electric Piano 1";
	}
	case 6: {
		return "Electric Piano 2";
	}
	case 7: {
		return "Harpsichord";
	}
	case 8: {
		return "Clavinet (Clavi)";
	}
	case 9: {
		return "Celesta";
	}
	case 10: {
		return "Glockenspiel";
	}
	case 11: {
		return "Music Box";
	}
	case 12: {
		return "Vibraphone";
	}
	case 13: {
		return "Marimba";
	}
	case 14: {
		return "Xylophone";
	}
	case 15: {
		return "Tubular Bells";
	}
	case 16: {
		return "Dulcimer";
	}
	case 17: {
		return "Drawbar Organ";
	}
	case 18: {
		return "Percussive Organ";
	}
	case 19: {
		return "Rock Organ";
	}
	case 20: {
		return "Church Organ";
	}
	case 21: {
		return "Reed Organ";
	}
	case 22: {
		return "Accordion";
	}
	case 23: {
		return "Harmonica";
	}
	case 24: {
		return "Tango Accordion";
	}
	case 25: {
		return "Acoustic Guitar (nylon)";
	}
	case 26: {
		return "Acoustic Guitar (steel)";
	}
	case 27: {
		return "Electric Guitar (jazz)";
	}
	case 28: {
		return "Electric Guitar (clean)";
	}
	case 29: {
		return "Electric Guitar (muted)";
	}
	case 30: {
		return "Overdriven Guitar";
	}
	case 31: {
		return "Distortion Guitar";
	}
	case 32: {
		return "Guitar harmonics";
	}
	case 33: {
		return "Acoustic Bass";
	}
	case 34: {
		return "Electric Bass (finger)";
	}
	case 35: {
		return "Electric Bass (pick)";
	}
	case 36: {
		return "Fretless Bass";
	}
	case 37: {
		return "Slap Bass 1";
	}
	case 38: {
		return "Slap Bass 2";
	}
	case 39: {
		return "Synth Bass 1";
	}
	case 40: {
		return "Synth Bass 2";
	}
	case 41: {
		return "Violin";
	}
	case 42: {
		return "Viola";
	}
	case 43: {
		return "Cello";
	}
	case 44: {
		return "Contrabass";
	}
	case 45: {
		return "Tremolo Strings";
	}
	case 46: {
		return "Pizzicato Strings";
	}
	case 47: {
		return "Orchestral Harp";
	}
	case 48: {
		return "Timpani";
	}
	case 49: {
		return "String Ensemble 1";
	}
	case 50: {
		return "String Ensemble 2";
	}
	case 51: {
		return "Synth Strings 1";
	}
	case 52: {
		return "Synth Strings 2";
	}
	case 53: {
		return "Choir Aahs";
	}
	case 54: {
		return "Voice Oohs";
	}
	case 55: {
		return "Synth Choir";
	}
	case 56: {
		return "Orchestra Hit";
	}
	case 57: {
		return "Trumpet";
	}
	case 58: {
		return "Trombone";
	}
	case 59: {
		return "Tuba";
	}
	case 60: {
		return "Muted Trumpet";
	}
	case 61: {
		return "French Horn";
	}
	case 62: {
		return "Brass Section";
	}
	case 63: {
		return "Synth Brass 1";
	}
	case 64: {
		return "Synth Brass 2";
	}
	case 65: {
		return "Soprano Sax";
	}
	case 66: {
		return "Alto Sax";
	}
	case 67: {
		return "Tenor Sax";
	}
	case 68: {
		return "Baritone Sax";
	}
	case 69: {
		return "Oboe";
	}
	case 70: {
		return "English Horn";
	}
	case 71: {
		return "Bassoon";
	}
	case 72: {
		return "Clarinet";
	}
	case 73: {
		return "Piccolo";
	}
	case 74: {
		return "Flute";
	}
	case 75: {
		return "Recorder";
	}
	case 76: {
		return "Pan Flute";
	}
	case 77: {
		return "Blown Bottle";
	}
	case 78: {
		return "Shakuhachi";
	}
	case 79: {
		return "Whistle";
	}
	case 80: {
		return "Ocarina";
	}
	case 81: {
		return "Lead 1 (square)";
	}
	case 82: {
		return "Lead 2 (sawtooth)";
	}
	case 83: {
		return "Lead 3 (calliope)";
	}
	case 84: {
		return "Lead 4 (chiff)";
	}
	case 85: {
		return "Lead 5 (charang)";
	}
	case 86: {
		return "Lead 6 (voice)";
	}
	case 87: {
		return "Lead 7 (fifths)";
	}
	case 88: {
		return "Lead 8 (bass + lead)";
	}
	case 89: {
		return "Pad 1 (new age)";
	}
	case 90: {
		return "Pad 2 (warm)";
	}
	case 91: {
		return "Pad 3 (polysynth)";
	}
	case 92: {
		return "Pad 4 (choir)";
	}
	case 93: {
		return "Pad 5 (bowed)";
	}
	case 94: {
		return "Pad 6 (metallic)";
	}
	case 95: {
		return "Pad 7 (halo)";
	}
	case 96: {
		return "Pad 8 (sweep)";
	}
	case 97: {
		return "FX 1 (rain)";
	}
	case 98: {
		return "FX 2 (soundtrack)";
	}
	case 99: {
		return "FX 3 (crystal)";
	}
	case 100: {
		return "FX 4 (atmosphere)";
	}
	case 101: {
		return "FX 5 (brightness)";
	}
	case 102: {
		return "FX 6 (goblins)";
	}
	case 103: {
		return "FX 7 (echoes)";
	}
	case 104: {
		return "FX 8 (sci-fi)";
	}
	case 105: {
		return "Sitar";
	}
	case 106: {
		return "Banjo";
	}
	case 107: {
		return "Shamisen";
	}
	case 108: {
		return "Koto";
	}
	case 109: {
		return "Kalimba";
	}
	case 110: {
		return "Bag pipe";
	}
	case 111: {
		return "Fiddle";
	}
	case 112: {
		return "Shanai";
	}
	case 113: {
		return "Tinkle Bell";
	}
	case 114: {
		return "Agogo";
	}
	case 115: {
		return "Steel Drums";
	}
	case 116: {
		return "Woodblock";
	}
	case 117: {
		return "Taiko Drum";
	}
	case 118: {
		return "Melodic Tom";
	}
	case 119: {
		return "Synth Drum";
	}
	case 120: {
		return "Reverse Cymbal";
	}
	case 121: {
		return "Guitar Fret Noise";
	}
	case 122: {
		return "Breath Noise";
	}
	case 123: {
		return "Seashore";
	}
	case 124: {
		return "Bird Tweet";
	}
	case 125: {
		return "Telephone Ring";
	}
	case 126: {
		return "Helicopter";
	}
	case 127: {
		return "Applause";
	}
	case 128: {
		return "Gunshot";
	}
	}
	return "out of range";
}





//------------------------------------------------------------------------------------
/**
* Check if a file is a MIDI file.
* @param filename Path to the file to check
* @return TRUE if it could be a MIDI file, FALSE otherwise
*
* The current implementation only checks for the "MThd" header in the file.
* It is useful only to distinguish between SoundFont and MIDI files.
*/
int CMidiFileHelper::ct_is_midifile(std::string claSFileName)
{
	FILE* fp = NULL;
	char id[4];
	int retcode = CT_FAILED;

	do
	{
		if ((0 != CT_FOPEN_S(&fp, claSFileName.c_str(), "rb")) || !fp)
		{
			CT_LOG(CT_ERR, "Unable to open file");
			return retcode;
		}

		if (CT_FREAD(&id, sizeof(id), 1, fp) != 1)
		{
			break;
		}

		if (id[0] == 'M' && id[1] == 'T' && id[2] == 'h' && id[3] == 'd')
			retcode = CT_OK;
	} while (0);

	CT_FCLOSE(fp);

	return retcode;
}







char* CMidiFileHelper::ct_file_read_full(ct_file fp, size_t* length)
{
	size_t buflen;
	char* buffer;
	size_t n;

	/* Work out the length of the file in advance */
	if (CT_FSEEK(fp, 0, SEEK_END) != 0)
	{
		CT_LOG(CT_ERR, "File load: Could not seek within file");
		return NULL;
	}

	buflen = ftell(fp);

	if (CT_FSEEK(fp, 0, SEEK_SET) != 0)
	{
		CT_LOG(CT_ERR, "File load: Could not seek within file");
		return NULL;
	}

	buffer = (char*)CT_MALLOC(buflen);

	if (buffer == NULL)
	{
		CT_LOG(CT_PANIC, "Out of memory");
		return NULL;
	}

	n = CT_FREAD(buffer, 1, buflen, fp);

	if (n != buflen)
	{
		CT_LOG(CT_ERR, "Only read %lu bytes; expected %lu", (unsigned long)n,
			(unsigned long)buflen);
		CT_FREE(buffer);
		return NULL;
	};

	*length = n;

	return buffer;
}







/*
 * ct_isasciistring
 */
int CMidiFileHelper::ct_isasciistring(char* s)
{
	/* From ctype.h */
#define ct_isascii(c)    (((c) & ~0x7f) == 0)

	size_t i, len = CT_STRLEN(s);

	for (i = 0; i < len; i++)
	{
		if (!ct_isascii(s[i]))
		{
			return 0;
		}
	}

	return 1;

#undef ct_isascii
}




/*
 * ct_getlength
 */
long CMidiFileHelper::ct_getlength(unsigned char* s)
{
	long i = 0;
	i = s[3] | (s[2] << 8) | (s[1] << 16) | (s[0] << 24);
	return i;
}




/*
 * ct_midi_file_read_mthd
 */
int CMidiFileHelper::ct_midi_file_read_mthd(CMidiFile* mf)
{
	auto mthd = mf->cp_HeaderBuffer;

	if (mf->fnReadBuffer(mthd, MIDI_FILE_HEADER_SIZE) != CT_OK)
	{
		return CT_FAILED;
	}

	if ((CT_STRNCMP(mthd, "MThd", 4) != 0) || (mthd[7] != 6) || (mthd[9] > 2))
	{
		CT_LOG(CT_ERR,
			"Doesn't look like a MIDI file: invalid MThd header");
		return CT_FAILED;
	}

	mf->i_MidiFleType = mthd[9];
	mf->i_TracksInFile = (unsigned)mthd[11];
	mf->i_TracksInFile += (unsigned int)(mthd[10]) << 8;

	if ((signed char)mthd[12] < 0)
	{
		CT_LOG(CT_ERR, "File uses SMPTE timing -- Not implemented yet");
		return CT_FAILED;
	}
	else
	{
		mf->ui_DivisionPerQuarter = ((unsigned)mthd[12] << 8) | ((unsigned)mthd[13] & 0xff);
	}

	return CT_OK;
}



//------------------------------------------------------------------------------------
//
int CMidiFileHelper::ct_midi_file_load_tracks()
{
	ct_return_val_if_fail(clp_MidiFile != NULL, CT_FAILED);

	for (int i = 0; i < clp_MidiFile->i_TracksInFile; i++)
	{
		if (ct_midi_file_read_track(i) != CT_OK)
			return CT_FAILED;
	}

	return CT_OK;
}



//------------------------------------------------------------------------------------
//
int CMidiFileHelper::ct_midi_file_read_tracklen()
{
	ct_return_val_if_fail(clp_MidiFile != NULL, CT_FAILED);

	unsigned char length[5];

	if (clp_MidiFile->fnReadBuffer(length, 4) != CT_OK)
	{
		return CT_FAILED;
	}

	return ct_getlength(length);
}




//------------------------------------------------------------------------------------
//
int CMidiFileHelper::ct_midi_file_read_track(int num)
{
	ct_return_val_if_fail(clp_MidiFile != NULL, CT_FAILED);

	unsigned char id[5], length[5];
	int found_track = 0;
	int skip;

	if (clp_MidiFile->fnReadBuffer(id, 4) != CT_OK)
	{
		return CT_FAILED;
	}

	id[4] = '\0';


	while (!found_track)
	{
		if (ct_isasciistring((char*)id) == 0)
		{
			CT_LOG(CT_ERR,
				"An non-ascii track header found, corrupt file");
			return CT_FAILED;

		}
		else if (CT_STRCMP((char*)id, "MTrk") == 0)
		{
			const int niTrackStartPos = clp_MidiFile->i_BufferCurPos - 4; // For "MTrk"
			found_track = 1;

			const int niTrackLength = ct_midi_file_read_tracklen();
			if ( CT_FAILED == niTrackLength )
			{
				return CT_FAILED;
			}

			CMidiTrack* clpMidiTrack = new CMidiTrack(num, niTrackStartPos, niTrackLength, clp_MidiFile->i_BufferCurPos - niTrackStartPos);
			if ( !clpMidiTrack)
			{
				CT_LOG(CT_ERR, "Out of memory");
				return CT_FAILED;
			}



			// Now lets create the buffer for the track parsing
			clp_MidiFile->running_status = -1;
			clp_MidiFile->i_c = -1 ;
			clp_MidiFile->eot = 0 ;
			
			int iCurrentTickPos = 0;

			while (!clp_MidiFile->fnIsEOT())
			{
				/* read the delta-time of the event */
				int iTickCount = clp_MidiFile->fnGetVariableLengthValue();
				if ( CT_FAILED == iTickCount )
				{
					delete clpMidiTrack;
					clpMidiTrack = nullptr;
					return CT_FAILED;
				}
				iCurrentTickPos += iTickCount;

				// Now lets get the new event.
				bool bOK = true;
				CBaseMidiEvent* clpEvent = clp_MidiFile->fnReadEvent( bOK , clpMidiTrack );
				if (!bOK)
				{
					delete clpMidiTrack;
					clpMidiTrack = nullptr;
					return CT_FAILED;
				}

				if (clpEvent)
				{
					clpEvent->fnSetEventTickPos(iCurrentTickPos);
					clpMidiTrack->fnAddMidiEvent(iCurrentTickPos, clpEvent);
				}
			}

			if (clp_MidiFile->fnAddTrack(clpMidiTrack, iCurrentTickPos) != CT_OK)
			{
				delete clpMidiTrack;
				clpMidiTrack = nullptr;
				return CT_FAILED;
			}
		}
		else
		{
			found_track = 0;

			if (clp_MidiFile->fnReadBuffer(length, 4) != CT_OK)
			{
				return CT_FAILED;
			}

			skip = ct_getlength(length);

			/* fseek(mf->fp, skip, SEEK_CUR); */
			if (clp_MidiFile->fnSkipBuffer(skip) != CT_OK)
			{
				return CT_FAILED;
			}
		}
	}

	if (clp_MidiFile->fnIsEOF())
	{
		CT_LOG(CT_ERR, "Unexpected end of file");
		return CT_FAILED;
	}

	return CT_OK;
}










//------------------------------------------------------------------------------------
//
bool CMidiFileHelper::fnInitialiseHelper(std::string claSFilePath)
{
	// delete the older file.
	if (clp_MidiFile)
		delete clp_MidiFile;
	clp_MidiFile = nullptr;

	char* buffer = NULL;
	size_t buffer_length = 0;

	{
		ct_file fp;
		/* This file is specified by filename; load the file from disk */

		/* Read the entire contents of the file into the buffer */
		if ((0 != CT_FOPEN_S(&fp, claSFilePath.c_str(), "rb")) || !fp)
		{
			CT_LOG(CT_ERR, "Couldn't open the MIDI file");
			return false;
		}

		buffer = ct_file_read_full(fp, &buffer_length);

		CT_FCLOSE(fp);

		if (buffer == NULL)
			return false;
	}

	clp_MidiFile = new CMidiFile(buffer, buffer_length);
	if (NULL == clp_MidiFile)
	{
		CT_FREE(buffer);
		return false;
	}
	if (ct_midi_file_read_mthd(clp_MidiFile) != CT_OK)
	{
		delete clp_MidiFile;
		clp_MidiFile = nullptr;
		return false;
	}

	if (ct_midi_file_load_tracks() != CT_OK)
	{
		delete clp_MidiFile;
		clp_MidiFile = nullptr;
		return false;
	}

	// We have successfully readed the file. Hence return true..
	return true;
}




static std::string get_channel_used_string(BOOL* bpaFlags, int iaCount)
{
	std::string clRetStr;
	for (int iI = 0; iI < iaCount; iI++)
	{
		if (FALSE == bpaFlags[iI])
			continue;

		char buff[32];
		if (clRetStr.size() < 1)
			snprintf(buff, sizeof(buff), "%d", iI);
		else
			snprintf(buff, sizeof(buff), ", %d", iI);

		clRetStr += buff;
	}

	return clRetStr;
}

//------------------------------------------------------------------------------------
//
void CMidiFileHelper::fnDisplaySettings()
{
	if (!clp_MidiFile)
	{
		CT_LOG(CT_ERR, "No valid Midi file is loaded");
		return;
	}

	CT_LOG(CT_INFO, "*****************   FILE INFO   ************************");
	CT_LOG(CT_INFO, "File Type        : %d", clp_MidiFile->i_MidiFleType);
	CT_LOG(CT_INFO, "Number of Tracks : %d", clp_MidiFile->i_TracksInFile);
	// Now lets write the channels that were used in this file.
	{
		std::string clChannelUsed = get_channel_used_string(&clp_MidiFile->bp_ChannelUsed[0], MAX_NUMBER_OF_CHANNELS);
		if (!clChannelUsed.empty())
			CT_LOG(CT_INFO, "Channels used : %s", clChannelUsed.c_str());
	}
	CT_LOG(CT_INFO, "");

	// Now lets fetch the track details.
	for (int iI = 0; iI < clp_MidiFile->i_TracksInFile; iI++)
	{
		if (clp_MidiFile->clpp_Track[iI])
			fnDisplayTrackSettings(clp_MidiFile->clpp_Track[iI]);
	}

	// Now lets show the Temp Settings
	CT_LOG(CT_INFO, "########################################################");
	for (auto clEventEntry : clp_MidiFile->cl_TempoMapEvent)
	{
		int iTick = clEventEntry.first;
		CTempoChangeMidiEvent* clpTempoMidiEvent = clEventEntry.second;
		CT_LOG(CT_INFO, "Tempo Changed To : %d - Time : %s", clpTempoMidiEvent->fnGetBPM(), clp_MidiFile->fnGetTimeDecoratedStringFromTick(iTick).c_str());
	}
	CT_LOG(CT_INFO, "########################################################");
	CT_LOG(CT_INFO, "");
}




//------------------------------------------------------------------------------------
// Added by AmitRungta on 18-09-2020
//
void CMidiFileHelper::fnDisplayTrackSettings(CMidiTrack* clpaTrack)
{
	if (!clpaTrack)
		return;

	CT_LOG(CT_INFO, "--------------------------------------------------------");
	CT_LOG(CT_INFO, "\tTrack Number : %d", clpaTrack->fnGetTrackNum());
	if (clpaTrack->fnGetTrackName().length() > 0 )
		CT_LOG(CT_INFO, "\tTrack Name   : %s", clpaTrack->fnGetTrackName().c_str());




	// Now lets parse all the channels and the Program change event for this track.
	int iDefaultChannel = -1;

	for (auto clEventEntry : *clpaTrack->fnGetEventMap())
	{
		int iTick = clEventEntry.first;
		CBaseMidiEvent* clpBaseMidiEvent = clEventEntry.second;

		if (clpBaseMidiEvent->fnIsChannelBasedEvent())
		{
			CChannelBasedMidiEvent* clpChannelBasedEvent = dynamic_cast<CChannelBasedMidiEvent*>(clpBaseMidiEvent);
			if (clpChannelBasedEvent)
			{
				if (PROGRAM_CHANGE == clpChannelBasedEvent->fnGetEventType())
				{
					CT_LOG(CT_INFO, "\t\tProgram changed for Channel(%d) to : %s (%d) - Time : %s"
						, clpChannelBasedEvent->fnGetEventChannel()
						, fnGetInstrumentName(clpChannelBasedEvent->fnGetParam1()).c_str()
						, clpChannelBasedEvent->fnGetParam1()
						, clp_MidiFile->fnGetTimeDecoratedStringFromTick(iTick).c_str());
					iDefaultChannel = clpChannelBasedEvent->fnGetEventChannel();
				}
				else if (iDefaultChannel < 0 && NOTE_ON == clpChannelBasedEvent->fnGetEventType())
				{
					CT_LOG(CT_INFO, "\t\tDefault Channel to : %d - Time : %s", clpChannelBasedEvent->fnGetEventChannel(), clp_MidiFile->fnGetTimeDecoratedStringFromTick(iTick).c_str());
					iDefaultChannel = clpChannelBasedEvent->fnGetEventChannel();
				}
			}
		}
	}

	// Now lets write the channels that were used in this track.
	{
		std::string clChannelUsed = get_channel_used_string(clpaTrack->fnGetChannelUsedPtr(), MAX_NUMBER_OF_CHANNELS);
		if (clChannelUsed.empty())
		{
			CT_LOG(CT_INFO, "\tNo channels used in this Track");
		}
		else
		{
			CT_LOG(CT_INFO, "\tChannels used in Track : %s" , clChannelUsed.c_str());
		}
	}

	CT_LOG(CT_INFO, "");
}



//------------------------------------------------------------------------------------
//
bool CMidiFileHelper::fnSaveTempoSettingsAsJson(std::string claSJsonFilePath)
{
	if (!clp_MidiFile)
	{
		CT_LOG(CT_ERR, "No valid Midi file is loaded");
		return false;
	}

	if (claSJsonFilePath.empty())
	{
		CT_LOG(CT_ERR, "Json file path to save is empty");
		return false;
	}

	std::ofstream clOutFile;
	clOutFile.open(claSJsonFilePath);
	if (clOutFile.fail())
	{
		CT_LOG(CT_ERR, "Error in opening the json file for writing.");
		return false;
	}
	
	{
		json clTempoJson;
		clTempoJson[JSON_BPM_TAG_NAME] = json::object();
		auto& clTempObject = clTempoJson[JSON_BPM_TAG_NAME];

		for (auto clEventEntry : clp_MidiFile->cl_TempoMapEvent)
		{
			int iTick = clEventEntry.first;
			CTempoChangeMidiEvent* clpTempoMidiEvent = clEventEntry.second;
			std::string clTimeStr = clp_MidiFile->fnGetTimeDecoratedStringFromTick(iTick);
			clTempObject[clTimeStr] = clpTempoMidiEvent->fnGetBPM();
		}

		clOutFile << std::setw(4) << clTempoJson << std::endl;
	}
	clOutFile.close();

	return true;
}







//------------------------------------------------------------------------------------
//
ct_EventBuffer CMidiFileHelper::fnGenerateFileForChannel(int iaChannelNo, std::string claSJsonFilePath)
{
	ct_EventBuffer clBufToRet;

	if (!clp_MidiFile)
	{
		CT_LOG(CT_ERR, "No valid Midi file is loaded");
		return clBufToRet;
	}

	if (iaChannelNo < 0 || iaChannelNo >= MAX_NUMBER_OF_CHANNELS)
	{
		CT_LOG(CT_ERR, "Selected channel number is not in Valid range");
		return clBufToRet;
	}

	// Now check if the midi file contains the channel data or not.
	if (!clp_MidiFile->bp_ChannelUsed[iaChannelNo])
	{
		CT_LOG(CT_ERR, "Selected channel doesnot contain any valid data for playing. Select a valid channel number.");
		return clBufToRet;
	}

	// Now lets create a list of all the tracks whose data needs to be copied to the desired output.
	std::vector < CMidiTrack*> clTracksToUseVector;
	for (int iI = 0; iI < clp_MidiFile->i_TracksInFile; iI++)
	{
		auto track = clp_MidiFile->clpp_Track[iI];
		if (!track)
			continue;

		bool bUseChannel = false;
		if ( track->fnIsChannelUsed(iaChannelNo))
		{
			bUseChannel = true;
		}
		else
		{
			// This track should not have any valid channel data. Should just contain global data. then add it.
			BOOL bContainsData = FALSE;
			for (int iCha = 0; iCha < MAX_NUMBER_OF_CHANNELS; iCha++)
				bContainsData |= track->fnIsChannelUsed(iCha);
			if (FALSE == bContainsData)
				bUseChannel = true;
		}

		if (bUseChannel)
			clTracksToUseVector.push_back(track);
	}

	if (clTracksToUseVector.size() < 1)
	{
		_ASSERT(FALSE);
		CT_LOG(CT_ERR, "Error in processing the MIDI file for channels.");
		return clBufToRet;
	}

	// Lets first get the default size.
	int iTempBufferSize = MIDI_FILE_HEADER_SIZE;
	for (auto track : clTracksToUseVector)
		iTempBufferSize += ( track->fnGetEventStartPosInTrack() + track->fnGetTrackLength() ) ;


	// Just create a Dummy placeholder. Actual size may differ.
	clBufToRet.reserve(iTempBufferSize);

	// Now lets first copy the Header after modifying the track number count.
	{
		// First copy the buffer as it is.
		clBufToRet.insert(clBufToRet.end(), &clp_MidiFile->ucp_buffer[0], &clp_MidiFile->ucp_buffer[MIDI_FILE_HEADER_SIZE]);

		// Now lets update the track number count.
		int16_t trackCount = clTracksToUseVector.size();
		clBufToRet[10] = (unsigned)( (trackCount & 0xff00) >> 8 ) ;
		clBufToRet[11] = (unsigned)(trackCount & 0x00ff);
	}

	// Now lets copy the tracks one by one.
	for (auto track : clTracksToUseVector)
	{
		if (!fnCopyTrackDataForChannel(&clBufToRet, track, iaChannelNo, claSJsonFilePath))
		{
			_ASSERT(FALSE);
			CT_LOG(CT_ERR, "Error in processing the MIDI file for channels.");
			clBufToRet.clear();
			return clBufToRet;
		}
	}

	return clBufToRet;
}




//------------------------------------------------------------------------------------
//
bool CMidiFileHelper::fnCopyTrackDataForChannel(ct_EventBuffer* clpaEventBuffer, CMidiTrack* orgtrack, int iaChannelNo, std::string claSJsonFilePath)
{
	// Now check if any other channel than the current one is selected then do special processing oherwise copy the buffer directly.
	bool bContainsOtherChannelData = false;
	for (int iCha = 0; iCha < MAX_NUMBER_OF_CHANNELS; iCha++)
	{
		if (orgtrack->fnIsChannelUsed(iCha) && iCha != iaChannelNo)
		{
			bContainsOtherChannelData = true;
			break;
		}
	}

	bool bContainsTempoData = false;
	if (!claSJsonFilePath.empty())
	{
		for (auto clEventEntry : *orgtrack->fnGetEventMap())
		{
			auto clpBaseEvent = clEventEntry.second;
			if (clpBaseEvent && clpBaseEvent->fnGetEventType() == MIDI_SET_TEMPO)
			{
				bContainsTempoData = true;
				break;
			}
		}
	}

	if (bContainsTempoData)
	{
		std::map<int, int> clBpmLookupMap;

		// Lets first of all add a default BPM, so that if none BPM is present for start then we have some data.
		clBpmLookupMap[0] = 120 ;

		// Now lets try to read the json file and extract the tempo data from it.
		json clReadJsonObj;
		std::ifstream clInFile;
		clInFile.open(claSJsonFilePath);
		if (clInFile.fail())
		{
			CT_LOG(CT_ERR, "Error in opening json file for BPM data.");
			return false;
		}
		try
		{
			clInFile >> clReadJsonObj;
			auto& clTempoObj = clReadJsonObj.at(JSON_BPM_TAG_NAME);
			if (clTempoObj.size() < 1)
			{
				CT_LOG(CT_ERR, "BPM file does not contain any valid data.");
				clInFile.close();
				return false;
			}

			for (auto& clNode : clTempoObj.items())
			{
				std::string clsTime = clNode.key();
				auto clVal = clNode.value();
				if (!clVal.is_number())
				{
					CT_LOG(CT_ERR, "BPM file data is invalid for time %s", clsTime);
					clInFile.close();
					return false;
				}

				int iBPM = clNode.value().get<int>();
				int iTimeInMillisec = clp_MidiFile->fnGetMillisecondFromTimeDecoratedString(clsTime);
				clBpmLookupMap[iTimeInMillisec] = iBPM;
			}
		}
		catch (json::invalid_iterator& e)
		{
			CT_LOG(CT_ERR, "Error in reading BPM data %s", e.what());
			clInFile.close();
			return false;
		}
		clInFile.close();

		{
			// Added by AmitRungta on 29-09-2020  We will need to reset the EOT event. As in one cas it s at 0 pos and hence our new tempo events corrupted the files.
			CBaseMidiEvent* clpEOTEvent = nullptr;


			// Now lets first of all delete the existing tempo data.
			{
				auto clItrBegin = orgtrack->fnGetEventMap()->begin();
				auto clItrEnd = orgtrack->fnGetEventMap()->end();
				while (clItrBegin != clItrEnd)
				{
					auto clpEvent = clItrBegin->second;
					if (clpEvent && MIDI_SET_TEMPO == clpEvent->fnGetEventType())
					{
						clItrBegin = orgtrack->fnGetEventMap()->erase(clItrBegin);
						// Delete the the event ptr.
						delete clpEvent;
						clpEvent = nullptr;
					}
					else if (clpEvent && MIDI_EOT == clpEvent->fnGetEventType())
					{
						clItrBegin = orgtrack->fnGetEventMap()->erase(clItrBegin);
						// Save this event for later addition
						clpEOTEvent = clpEvent;
					}
					else
					{
						clItrBegin++;
					}
				}

				// Now lets clear the master file vector also.
				clp_MidiFile->cl_TempoMapEvent.clear();
			}

			// Now lets add the new tempo.
			{
				int iLastTickPos = 0;
				auto clItrBegin = clBpmLookupMap.begin();
				auto clItrEnd = clBpmLookupMap.end();
				while (clItrBegin != clItrEnd)
				{
					int iTimeInMillisec = clItrBegin->first;
					int iBPM = clItrBegin->second;
					clItrBegin++;
					
					
					int iTimeInTick = iTimeInMillisec > 0 ? clp_MidiFile->fnGetTickFromMillisecond(iTimeInMillisec) : 0 ;
					int iTempo = 60000000 / iBPM;

					if (iTimeInTick >= 0 && iTimeInTick < clp_MidiFile->i_MaxTicPosInFile)
					{
						CTempoChangeMidiEvent* clpTempo = new CTempoChangeMidiEvent(nullptr, 0, MIDI_SET_TEMPO, iTempo, 0);
						clpTempo->fnSetEventTickPos(iTimeInTick);
						orgtrack->fnAddMidiEvent(iTimeInTick, clpTempo);

						// Now lets add this to our master tempo list also.
						clp_MidiFile->cl_TempoMapEvent.insert(ct_TempoMap_Pair(iTimeInTick, clpTempo));

						if (iLastTickPos < iTimeInTick)
							iLastTickPos = iTimeInTick;
					}
				}

				// Now lets add the EOT event.
				if (clpEOTEvent)
				{
					int iDesiredEOTTickPos = iLastTickPos + 1;
					if (iDesiredEOTTickPos < clpEOTEvent->fnGetEventTickPos())
						iDesiredEOTTickPos = clpEOTEvent->fnGetEventTickPos();
					else
						clpEOTEvent->fnSetEventTickPos(iDesiredEOTTickPos);
					orgtrack->fnAddMidiEvent(iDesiredEOTTickPos, clpEOTEvent);
				}
			}
		}
	}


	// Now if this track does not contain tempo or other channel data then lets copy this track buffer as it is, otherwise we will create a new buffer from scratch
	if ( !bContainsOtherChannelData && !bContainsTempoData )
	{
		auto bufferstart = &clp_MidiFile->ucp_buffer[orgtrack->fnGetTrackStartPos()];
		clpaEventBuffer->insert(clpaEventBuffer->end(), bufferstart, bufferstart  + (orgtrack->fnGetEventStartPosInTrack() + orgtrack->fnGetTrackLength()));
	}
	else
	{
		bool bpSupportedChannels[MAX_NUMBER_OF_CHANNELS];
		for (int iI = 0; iI < MAX_NUMBER_OF_CHANNELS; iI++)
			bpSupportedChannels[iI] = false;
		bpSupportedChannels[iaChannelNo] = true;


		// Let recreate the entire track..
		ct_EventBuffer clTrackBuffer;
		clTrackBuffer.reserve(orgtrack->fnGetEventStartPosInTrack() + orgtrack->fnGetTrackLength());			// Just a dummy reserve

		clTrackBuffer.push_back('M');
		clTrackBuffer.push_back('T');
		clTrackBuffer.push_back('r');
		clTrackBuffer.push_back('k');

		int trackLengthPos = clTrackBuffer.size();

		// Set the dummy track size here.
		clTrackBuffer.push_back('\0');
		clTrackBuffer.push_back('\0');
		clTrackBuffer.push_back('\0');
		clTrackBuffer.push_back('\0');

		unsigned int uiTrackBuffSize = 0;
		int iCurrentTick = 0;

		for (auto clEventEntry : *orgtrack->fnGetEventMap())
		{
			int iTick = clEventEntry.first;
			auto clpBaseEvent = clEventEntry.second;
			if ( !clpBaseEvent || !clpBaseEvent->fnIsSupportedForChannel(bpSupportedChannels))
				continue;

			// We need to write this event data.

			// Lets first write the delta time before the actual event data.
			int time = iTick - iCurrentTick;
			ct_EventBuffer deltaTime = clp_MidiFile->fnWriteVariableLengthValue(time);
			uiTrackBuffSize += deltaTime.size();
			clTrackBuffer.insert(clTrackBuffer.end(), deltaTime.begin(), deltaTime.end());


			// write the events data
			ct_EventBuffer clEventData = clpBaseEvent->fnGetBufferToSave();
			uiTrackBuffSize += clEventData.size();
			clTrackBuffer.insert(clTrackBuffer.end(), clEventData.begin(), clEventData.end());

			// save this tick as last time
			iCurrentTick = iTick;
		}

		// Now lets write back the track size.
		for (int i = 3; i >= 0; i--) {
			clTrackBuffer[trackLengthPos + 3 - i] = ((int8_t)((uiTrackBuffSize & (0xFF << 8 * i)) >> 8 * i));
		}

		if (clTrackBuffer.size() > 0)
		{
			clpaEventBuffer->insert(clpaEventBuffer->end(), clTrackBuffer.begin(), clTrackBuffer.end());
		}
	}


	return true;
}