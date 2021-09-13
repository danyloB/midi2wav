#pragma once

#include "MidiEvent.h"
#include "MidiHelperData.h"
#include <string>



//------------------------------------------------------------------------------------
//
class CMidiTrack
{
public:
    CMidiTrack( int iaNum , int iaTrackStartPos, int iaTrackLength , int iaEventStartPosInTrack);
    virtual ~CMidiTrack();

    virtual int fnGetTrackStartPos() { return i_TrackStartPos; };
    virtual int fnGetTrackLength() { return i_TrackLength; };
    virtual int fnGetEventStartPosInTrack() { return i_EventStartPosInTrack; };
    virtual int fnGetTrackNum() { return i_TrackNum; };

    virtual int fnAddMidiEvent(int iaTickPos, CBaseMidiEvent* clpaEvent);

    virtual BOOL fnIsChannelUsed(unsigned char ucaChannel) { return bp_ChannelUsed[ucaChannel]; };
    virtual BOOL* fnGetChannelUsedPtr() { return bp_ChannelUsed; };
    virtual ct_EventMap* fnGetEventMap() { return &cl_TrackEventMap; };

    std::string fnGetTrackName() { return cl_TrackName; };
    void fnSetTrackName(std::string claName ) { cl_TrackName = claName ; };



protected :
    BOOL bp_ChannelUsed[MAX_NUMBER_OF_CHANNELS];
    int i_TrackStartPos;
    int i_TrackLength;
    int i_EventStartPosInTrack;

    std::string cl_TrackName;
    int i_TrackNum;

    ct_EventMap cl_TrackEventMap;
};

