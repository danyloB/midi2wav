#include <stdio.h> // printf
#include <stdint.h>
#include <stdlib.h> // malloc, free
#include <stdarg.h>
#include <iostream>
#include <string>
#include "MidiTrack.h"
#include "MidiHelperData.h"



//------------------------------------------------------------------------------------
//
CMidiTrack::CMidiTrack(int iaNum, int iaTrackStartPos, int iaTrackLength, int iaEventStartPosInTrack)
{
	i_TrackNum = iaNum;
	i_TrackStartPos = iaTrackStartPos;
	i_TrackLength = iaTrackLength ;
	i_EventStartPosInTrack = iaEventStartPosInTrack ;
	for (int iI = 0; iI < MAX_NUMBER_OF_CHANNELS; iI++)
		bp_ChannelUsed[iI] = false;
}



//------------------------------------------------------------------------------------
//
CMidiTrack::~CMidiTrack()
{
	for (auto baseeventitem : cl_TrackEventMap)
	{
		auto clpBaseEvent = baseeventitem.second;
		delete clpBaseEvent;
	}
	cl_TrackEventMap.clear();
}



//------------------------------------------------------------------------------------
//
int CMidiTrack::fnAddMidiEvent(int iaTickPos, CBaseMidiEvent* clpaEvent)
{
	_ASSERT(iaTickPos == clpaEvent->fnGetEventTickPos());
	cl_TrackEventMap.insert(ct_EventMap_Pair(iaTickPos, clpaEvent));

	if (NOTE_ON == clpaEvent->fnGetEventType() && clpaEvent->fnGetParam2() > 0)
		bp_ChannelUsed[clpaEvent->fnGetEventChannel()] = TRUE;

	return CT_OK;
}

