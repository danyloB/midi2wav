// MidiExtractor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "MidiFileHelper.h"
#include <windows.h>

#include <fluidsynth.h>








//------------------------------------------------------------------------------------
//
static void print_help(bool bOnlyUsage)
{
	if (!bOnlyUsage)
	{
		std::cout << std::endl  << "***************************************************************" << std::endl;
		std::cout << "Converts / Display the current data in the MIDI file." << std::endl;
		std::cout << "***************************************************************" << std::endl;
	}

	std::cout << std::endl;
	std::cout << "    ------- Display MIDI file data -------" << std::endl << std::endl;
	std::cout << "Usage:midi-convert.exe [input-midi-file-name.mid]" << std::endl;
	std::cout << "The required arguments:" << std::endl;
	std::cout << "  [input-midi-file-name.mid]          ... an input Midi source file" << std::endl << std::endl << std::endl;

	std::cout << "    ------- Extract Channel data from MIDI file -------" << std::endl << std::endl;
	std::cout << "Usage:midi-convert.exe [input-midi-file-name.mid] [channel-number] [our-sound-font-file.sf2] [bpm.json]" << std::endl;
	std::cout << "The required arguments:" << std::endl;
	std::cout << "  [input-midi-file-name.mid]          ... an input Midi source file" << std::endl;
	std::cout << "  [channel-number]                    ... channel whose data is to be retained." << std::endl;
	std::cout << "  [our-sound-font-file.sf2]           ... an input SoundFont file to use" << std::endl;
	std::cout << "The optional arguments:" << std::endl;
	std::cout << "  [BPM.json]                          ... desired json file containing the new BPM settings." << std::endl;
}




//------------------------------------------------------------------------------------
//
static bool fast_render_loop(fluid_settings_t* settings, fluid_synth_t* synth, fluid_player_t* player)
{
	fluid_file_renderer_t* renderer;
	renderer = new_fluid_file_renderer(synth);

	if (!renderer)
	{
		false ;
	}

	while (fluid_player_get_status(player) == FLUID_PLAYER_PLAYING)
	{
		if (fluid_file_renderer_process_block(renderer) != FLUID_OK)
		{
			break;
		}
	}

	delete_fluid_file_renderer(renderer);
	return true;
}





//------------------------------------------------------------------------------------
//
static bool __gfnSaveMidiBufferAsWavFile( char* cpaBuffer , int iaBuferSize , const char* cpaWaveFilePath , const char* cpasoundfontFilePath , int iaDesiredBPM )
{
	// First of all Disable the logging.
#if defined(WIN32)
	/* Windows logs to stdout by default, so make sure anything
		* lower than PANIC is not printed either */
	fluid_set_log_function(FLUID_ERR, NULL, NULL);
	fluid_set_log_function(FLUID_WARN, NULL, NULL);
	fluid_set_log_function(FLUID_INFO, NULL, NULL);
	fluid_set_log_function(FLUID_DBG, NULL, NULL);
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
#endif

	if (cpasoundfontFilePath && !fluid_is_soundfont(cpasoundfontFilePath))
	{
		CT_LOG(CT_ERR, "%s is not a valid sound font file.", cpasoundfontFilePath);
		return false;
	}


	bool bRetVal = false;

	{
		fluid_settings_t* settings = NULL;
		fluid_synth_t* synth = NULL;
		fluid_player_t* player = NULL;


		/* create the settings */
		settings = new_fluid_settings();
		if (settings == NULL)
		{
			CT_LOG(CT_ERR, "Failed to create the fluid settings");
			goto cleanup;
		}

		fluid_settings_setstr(settings, "audio.file.name", cpaWaveFilePath);
		fluid_settings_setstr(settings, "player.timing-source", "sample");
		fluid_settings_setint(settings, "synth.lock-memory", 0);


		/* create the synthesizer */
		synth = new_fluid_synth(settings);
		if (synth == NULL)
		{
			CT_LOG(CT_ERR, "Failed to create the synthesizer");
			goto cleanup;
		}

		// Now lest load the coundfont if present.
		if (cpasoundfontFilePath)
		{
			if (fluid_synth_sfload(synth, cpasoundfontFilePath, 1) == -1)
			{
				CT_LOG(CT_ERR, "Failed to load the SoundFont %s" , cpasoundfontFilePath);
				goto cleanup;
			}

		}

		player = new_fluid_player(synth);
		if (player == NULL)
		{
			CT_LOG(CT_ERR, "Failed to create the player");
			goto cleanup;
		}

		/* add the midi file buffer */
		fluid_player_add_mem(player, cpaBuffer , iaBuferSize);

		if (iaDesiredBPM > 0 )
			fluid_player_set_bpm(player, iaDesiredBPM);

		/* start the player */
		fluid_player_play(player);

		/* fast rendering audio file to craete the wav file */
		fast_render_loop(settings, synth, player);

		bRetVal = true;

	cleanup:

		delete_fluid_player(player);
		delete_fluid_synth(synth);
		delete_fluid_settings(settings);
	}

	return bRetVal;
}



//------------------------------------------------------------------------------------
//
int main(int argc, char* argv[])
{
	std::string clSrcMidiFile;
	int iChannelToRetain = -1;
	std::string clsoundfontFilePath;
	std::string cldesiredOutputWavFilePath;
	std::string clDesiredBPM;
	std::string wav_filename;

	if (argc >= 2 )
		clSrcMidiFile = argv[1];


	if (argc >= 3)
		iChannelToRetain = atoi(argv[2]);

	if (argc >= 4)
		clsoundfontFilePath = argv[3];

	if (argc >= 5)
		clDesiredBPM = argv[4];

	if (argc >= 6)
	{
		print_help(false);
		return 0;
	}

	if (2 != argc && 4 != argc && 5 != argc)		// as BPM is optional
	{
		print_help(false);
		return 0;
	}

	if (CMidiFileHelper::ct_is_midifile(clSrcMidiFile) != CT_OK)
	{
		CT_LOG(CT_ERR, "%s is not a valid MIDI file", clSrcMidiFile.c_str());
		return 0;
	}

	CMidiFileHelper clFileHelper;
	if (!clFileHelper.fnInitialiseHelper(clSrcMidiFile))
		return  -1;

#ifndef _DEBUG
	if (2 == argc)
#endif // #ifndef _DEBUG
	{
		clFileHelper.fnDisplaySettings();

		// Now lets save the settings as json file.
		std::string clSJsonFile = clSrcMidiFile.substr(0, clSrcMidiFile.find_last_of('.')) + ".json";
		clFileHelper.fnSaveTempoSettingsAsJson(clSJsonFile);
	}


	if (!clsoundfontFilePath.empty())
	{
		// Now lets generate the buffer for the reduced MIDI File.
		ct_EventBuffer clReducedBuffer = clFileHelper.fnGenerateFileForChannel(iChannelToRetain, clDesiredBPM);

		wav_filename = clSrcMidiFile.substr(0, clSrcMidiFile.find_last_of('.')) + ".wav";

		// Now lets save this MIDI buffer as a Wav file.
		__gfnSaveMidiBufferAsWavFile((char*)&clReducedBuffer[0], clReducedBuffer.size(), wav_filename.c_str(), clsoundfontFilePath.c_str(), -1);
	}


	if (!wav_filename.empty())
	{
		std::cout << std::endl << "*****************************************************************" << std::endl;
		std::cout << "File \"" << clSrcMidiFile.c_str() << "\" channel " << iChannelToRetain << " data saved as \"" << wav_filename.c_str() << "\"" << std::endl;
	}

	return 0;
}
