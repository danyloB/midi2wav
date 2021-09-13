#pragma once
#include <vector>
#include <map> 
#include "MidiHelperData.h"

class CBaseMidiEvent;
class CTempoChangeMidiEvent;


typedef std::vector<unsigned char> ct_EventBuffer;
typedef std::multimap<int, CBaseMidiEvent*> ct_EventMap;
typedef std::pair <int, CBaseMidiEvent*> ct_EventMap_Pair;

typedef std::map<int, CTempoChangeMidiEvent*> ct_TempoMap;
typedef std::pair <int, CTempoChangeMidiEvent*> ct_TempoMap_Pair;

//------------------------------------------------------------------------------------
//
class CBaseMidiEvent
{
public:
    CBaseMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType , unsigned int uiaParam1, unsigned int uiaParam2);


    virtual ~CBaseMidiEvent();

    virtual ct_EventBuffer fnGetBufferToSave() { return ucp_Buffer; } 

    virtual int fnGetEventTickPos() { return i_EventTickPos; };
    virtual void fnSetEventTickPos(int iaPos) { i_EventTickPos = iaPos; };

    virtual unsigned char fnGetEventType() { return uc_EventType; };
    virtual void fnSetEventType(unsigned char ucaVal) { uc_EventType = ucaVal; };

    virtual unsigned int fnGetParam1() { return ui_Param1; };
    virtual void fnSetParam1(unsigned int uiaVal) { ui_Param1 = uiaVal; };

    virtual unsigned int fnGetParam2() { return ui_Param2; };
    virtual void fnSetParam2(unsigned int uiaVal) { ui_Param2 = uiaVal; };

    virtual unsigned char fnGetEventChannel() { _ASSERT(FALSE) ;  return 255; };
    virtual void fnSetEventChannel(unsigned char ucaVal) { _ASSERT(FALSE); };

    virtual bool fnIsChannelBasedEvent() { return false; };
    virtual bool fnIsSupportedForChannel(bool* bpaSupportedChannel) { return true; }


protected :
    ct_EventBuffer ucp_Buffer;
    int i_EventTickPos;
    unsigned char uc_EventType;
    unsigned int ui_Param1;
    unsigned int ui_Param2;
};




//------------------------------------------------------------------------------------
//
class CGenericMidiEvent : public CBaseMidiEvent
{
public:
	CGenericMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType, unsigned int uiaParam1, unsigned int uiaParam2);
	~CGenericMidiEvent();

private:

};




//------------------------------------------------------------------------------------
//
class CChannelBasedMidiEvent : public CBaseMidiEvent
{
public:
    CChannelBasedMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType, unsigned int uiaParam1, unsigned int uiaParam2, unsigned char ucaEventChannel);
    ~CChannelBasedMidiEvent();


    virtual unsigned char fnGetEventChannel() { return uc_EventChannel; };
    virtual void fnSetEventChannel(unsigned char ucaVal) { uc_EventChannel = ucaVal; };

    virtual bool fnIsChannelBasedEvent() { return true; };
    virtual bool fnIsSupportedForChannel(bool* bpaSupportedChannel) { return bpaSupportedChannel[uc_EventChannel]; }


private:
    unsigned char uc_EventChannel;
};




//------------------------------------------------------------------------------------
//
class CTempoChangeMidiEvent : public CGenericMidiEvent
{
public:
    CTempoChangeMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType, unsigned int uiaParam1, unsigned int uiaParam2);
    ~CTempoChangeMidiEvent();

private:
    int i_Beats;


public :
    virtual ct_EventBuffer fnGetBufferToSave();


public :
    double fnGetMilliSecPerTick(unsigned int uiaDivisionPerQuarter);

    int fnGetBPM() { return i_Beats; };
    void fnSetBPM( int iaVal ) { i_Beats = iaVal ; };
};




