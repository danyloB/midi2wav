#pragma once

#include "MidiEvent.h"
#include "MidiTrack.h"
#include "MidiHelperData.h"
#include <string>






//------------------------------------------------------------------------------------
//
class CMidiFile
{
public:
    CMidiFile( char* cpaBuffer , int iaBufferSize );
    virtual ~CMidiFile();


public :
    virtual int fnReadBuffer(void* buf, int len);
    virtual int fnSkipBuffer(int skip);
    virtual int fnIsEOF() { return eof; };

    int fnAddTrack(CMidiTrack* clpaTrack , int iaMaxTickPosForTrack );

    CBaseMidiEvent* fnReadEvent( bool& barOK, CMidiTrack* clpaTrack);

    // Function to get millisecond for the tick point.
    int fnGetMillisecondFromTick(int iaTick);
    int fnGetTickFromMillisecond(int iaTimeMS);
    std::string fnGetTimeDecoratedStringFromTick(int iaTick);
    int fnGetTickFromTimeDecoratedString(std::string claSTime);
    int fnGetMillisecondFromTimeDecoratedString(std::string claSTime);



public :
    char* ucp_buffer;           /* Entire contents of MIDI file (borrowed) */
    int i_BufferSize;                  /* Length of buffer, in bytes */
    int i_BufferCurPos;                  /* Current read position in contents buffer */
    int eof;                      /* The "end of file" condition */
    int i_MidiFleType;
    int i_TracksInFile;
    unsigned int ui_DivisionPerQuarter;       /* division is ticks per beat (quarter-note) */
    int i_MaxTicPosInFile;

    CMidiTrack* clpp_Track[MAX_NUMBER_OF_TRACKS];
    BOOL bp_ChannelUsed[MAX_NUMBER_OF_CHANNELS];
    char cp_HeaderBuffer[MIDI_FILE_HEADER_SIZE];

    ct_TempoMap cl_TempoMapEvent;           // This will just contain the refernce to the original temp events 


public : 
    // Used when readting the track buffer.
    int running_status;
    int i_c;
    int eot;

    int fnGetVariableLengthValue();
    ct_EventBuffer fnWriteVariableLengthValue( int iaVal );

    int fnGetc();
    int fnPutc(int c);
    int fnIsEOT();


};

