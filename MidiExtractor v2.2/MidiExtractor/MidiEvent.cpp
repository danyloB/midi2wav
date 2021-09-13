#include <stdio.h> // printf
#include <stdint.h>
#include <stdlib.h> // malloc, free
#include <stdarg.h>
#include <iostream>
#include <string>
#include "MidiEvent.h"
#include "MidiHelperData.h"



//------------------------------------------------------------------------------------
//
CBaseMidiEvent::CBaseMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType, unsigned int uiaParam1, unsigned int uiaParam2)
{
	if (ucpSrcBuffer && iaSize > 0)
	{
		ucp_Buffer.reserve(iaSize);
		ucp_Buffer.insert(ucp_Buffer.end(), ucpSrcBuffer, ucpSrcBuffer + iaSize);
	}
	uc_EventType = ucaEventType;
	i_EventTickPos = 0;
	ui_Param1 = uiaParam1;
	ui_Param2 = uiaParam2;
}



//------------------------------------------------------------------------------------
//
CBaseMidiEvent::~CBaseMidiEvent()
{
}




//------------------------------------------------------------------------------------
//
CGenericMidiEvent::CGenericMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType, unsigned int uiaParam1, unsigned int uiaParam2) :
	CBaseMidiEvent(ucpSrcBuffer, iaSize, ucaEventType, uiaParam1, uiaParam2)
{

}




//------------------------------------------------------------------------------------
//
CGenericMidiEvent::~CGenericMidiEvent()
{

}




//------------------------------------------------------------------------------------
//
CChannelBasedMidiEvent::CChannelBasedMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType, unsigned int uiaParam1, unsigned int uiaParam2, unsigned char ucaEventChannel) :
	CBaseMidiEvent(ucpSrcBuffer, iaSize, ucaEventType, uiaParam1, uiaParam2)
{
	uc_EventChannel = ucaEventChannel;
}




//------------------------------------------------------------------------------------
//
CChannelBasedMidiEvent::~CChannelBasedMidiEvent()
{

}




//------------------------------------------------------------------------------------
//
CTempoChangeMidiEvent::CTempoChangeMidiEvent(unsigned char* ucpSrcBuffer, int iaSize, unsigned char ucaEventType, unsigned int uiaParam1, unsigned int uiaParam2) :
	CGenericMidiEvent(ucpSrcBuffer, iaSize, ucaEventType, uiaParam1, uiaParam2)
{
	if (uiaParam1 > 0)
		i_Beats = 60000000 / uiaParam1;
	else
		i_Beats = 96;
}




//------------------------------------------------------------------------------------
//
CTempoChangeMidiEvent::~CTempoChangeMidiEvent()
{

}



//------------------------------------------------------------------------------------
//
double CTempoChangeMidiEvent::fnGetMilliSecPerTick(unsigned int uiaDivisionPerQuarter)
{
	double dQuartersPerSecond = (double)i_Beats / 60;
	double dTicksPerSecond = (double)(uiaDivisionPerQuarter) *dQuartersPerSecond;
	return 1000 / (dTicksPerSecond);
}



//------------------------------------------------------------------------------------
//
ct_EventBuffer CTempoChangeMidiEvent::fnGetBufferToSave()
{
	ct_EventBuffer ucpBuffer;

	ucpBuffer.push_back(0xFF);
	ucpBuffer.push_back(MIDI_SET_TEMPO);
	ucpBuffer.push_back(0x03);
	int value = 60000000 / i_Beats;
	for (int i = 2; i >= 0; i--) {
		ucpBuffer.push_back((value & (0xFF << 8 * i)) >> 8 * i);
	}

	return ucpBuffer;
}


