#pragma once


#include <string>
#include "MidiHelperData.h"
#include "MidiFile.h"
#include "MidiTrack.h"
#include "MidiEvent.h"









//------------------------------------------------------------------------------------
// Added by AmitRungta For implementing the Midi File Reader 
//
class CMidiFileHelper
{
public :
	CMidiFileHelper();
	~CMidiFileHelper();


public :
	bool fnInitialiseHelper(std::string claSFilePath);
	void fnDisplaySettings();
	bool fnSaveTempoSettingsAsJson( std::string claSJsonFilePath);
	ct_EventBuffer fnGenerateFileForChannel(int iaChannelNo, std::string claSJsonFilePath);


protected :
	void fnDisplayTrackSettings(CMidiTrack* clpaTrack);
	bool fnCopyTrackDataForChannel(ct_EventBuffer* clpaEventBuffer, CMidiTrack* track, int iaChannelNo, std::string claSJsonFilePath);

protected :
	int ct_midi_file_load_tracks();
	int ct_midi_file_read_track(int num);
	int ct_midi_file_read_tracklen();


protected :
	CMidiFile* clp_MidiFile;



protected:
	// TO generate the Midid file data from the buffer..
	static char* ct_file_read_full(ct_file fp, size_t* length);


	static int ct_midi_file_read_mthd(CMidiFile* midifile);

	static int ct_isasciistring(char* s);
	static long ct_getlength(unsigned char* s);



public:


	// Helper function to get the selected instrument name.
	static std::string fnGetInstrumentName(int iaProgID);

	// Check if the file is a valid midi file or not.
	static int ct_is_midifile(std::string claSFileName );
};

