//
// Copyright 2020 Electronic Arts Inc.
//
// TiberianDawn.DLL and RedAlert.dll and corresponding source code is free 
// software: you can redistribute it and/or modify it under the terms of 
// the GNU General Public License as published by the Free Software Foundation, 
// either version 3 of the License, or (at your option) any later version.

// TiberianDawn.DLL and RedAlert.dll and corresponding source code is distributed 
// in the hope that it will be useful, but with permitted additional restrictions 
// under Section 7 of the GPL. See the GNU General Public License in LICENSE.TXT 
// distributed with this program. You should have received a copy of the 
// GNU General Public License along with permitted additional restrictions 
// with this program. If not, see https://github.com/electronicarts/CnC_Remastered_Collection

/* $Header:   F:\projects\c&c\vcs\code\audio.cpv   2.17   16 Oct 1995 16:50:20   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AUDIO.CPP                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : September 10, 1993                                           *
 *                                                                                             *
 *                  Last Update : May 4, 1995 [JLB]                                            *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Is_Speaking -- Checks to see if the eva voice is still playing.                           *
 *   Sound_Effect -- General purpose sound player.                                             *
 *   Sound_Effect -- Plays a sound effect in the tactical map.                                 *
 *   Speak -- Computer speaks to the player.                                                   *
 *   Speak_AI -- Handles starting the EVA voices.                                              *
 *   Stop_Speaking -- Forces the EVA voice to stop talking.                                    *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include	"function.h"
#include "audio.h"




/*
** 
** 
** Win32lib stubs
** 
** 
** 
*/


SFX_Type					SoundType;
Sample_Type				SampleType;

int File_Stream_Sample(char const *filename, BOOL real_time_start) {return 1;};
int File_Stream_Sample_Vol(char const *filename, int volume, BOOL real_time_start)  {return 1;};
void __cdecl Sound_Callback(void) {};
void __cdecl far maintenance_callback(void) {};
void *Load_Sample(char const *filename) {return NULL;};
long Load_Sample_Into_Buffer(char const *filename, void *buffer, long size) {return 0;}
long Sample_Read(int fh, void *buffer, long size) {return 0;};
void Free_Sample(void const *sample) {};
BOOL Audio_Init( HWND window , int bits_per_sample, BOOL stereo , int rate , int reverse_channels) {return 0;};
void Sound_End(void) {};
void Stop_Sample(int handle) {};
BOOL Sample_Status(int handle) {return 0;};
BOOL Is_Sample_Playing(void const * sample) {return 0;};
void Stop_Sample_Playing(void const * sample) {};
int Play_Sample(void const *sample, int priority, int volume, signed short panloc) {return 1;};
int Play_Sample_Handle(void const *sample, int priority, int volume, signed short panloc, int id) {return 1;};
int Set_Sound_Vol(int volume) {return 0;};
int Set_Score_Vol(int volume) {return 0;};
void Fade_Sample(int handle, int ticks) {};
int Get_Free_Sample_Handle(int priority) {return 1;};
int Get_Digi_Handle(void) {return 1;}
long Sample_Length(void const *sample) {return 0;};
void Restore_Sound_Buffers (void) {};
BOOL Set_Primary_Buffer_Format(void) {return 0;};
BOOL Start_Primary_Sound_Buffer (BOOL forced) {return 0;};
void Stop_Primary_Sound_Buffer (void) {};











//
// External handlers. ST - 2/20/2019 3:41PM
//
extern void On_Sound_Effect(int sound_index, int variation, COORDINATE coord);
// extern void On_Speech(int speech_index); // MBL 02.06.2020
extern void On_Speech(int speech_index, HouseClass *house);
extern void On_Ping(const HouseClass* player_ptr, COORDINATE coord);

/***********************************************************************************************
 * Sound_Effect -- Plays a sound effect in the tactical map.                                   *
 *                                                                                             *
 *    This routine is used when a sound effect occurs in the game world. It handles fading     *
 *    the sound according to distance.                                                         *
 *                                                                                             *
 * INPUT:   voc   -- The sound effect number to play.                                          *
 *                                                                                             *
 *          coord -- The world location that the sound originates from.                        *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   01/05/1995 JLB : Reduces sound more dramatically when off screen.                         *
 *=============================================================================================*/
void Sound_Effect(VocType voc, COORDINATE coord, int variation)
{
	//
	// Intercept sound effect calls. ST - 2/20/2019 3:37PM
	//
	On_Sound_Effect((int)voc, variation, coord);
			
#if (0)			
	unsigned			distance;
	CELL				cell_pos;
	int pan_value;

	if (!Options.Volume || voc == VOC_NONE || !SoundOn || SampleType == SAMPLE_NONE) {
		return;
	}
	if (coord) {
		cell_pos = Coord_Cell(coord);
	}

	distance  = 0xFF;
	pan_value = 0;
	if (coord && !Map.In_View(cell_pos)) {
		distance = Map.Cell_Distance(cell_pos, Coord_Cell(Map.TacticalCoord));
		distance = (unsigned int)MIN((int)distance, (int)MAP_CELL_W);
		distance = Cardinal_To_Fixed(MAP_CELL_W, distance);
		distance = MIN(distance, 0xFFu);
		distance ^= 0xFF;

		distance /= 2;
		distance = MAX(distance, 25U);

		pan_value  = Cell_X(cell_pos);
		pan_value -= Coord_XCell(Map.TacticalCoord) + (Lepton_To_Cell(Map.TacLeptonWidth) >> 1);
		if (ABS(pan_value) > Lepton_To_Cell(Map.TacLeptonWidth >> 1)) {
			pan_value *= 0x8000;
			pan_value /= (MAP_CELL_W >> 2);
			pan_value = Bound(pan_value, -0x7FFF, 0x7FFF);
//			pan_value  = MAX((int)pan_value, (int)-0x7FFF);
//			pan_value  = MIN((int)pan_value, 0x7FFF);
		} else {
			pan_value  = 0;
		}
	}

	Sound_Effect(voc, (VolType)Fixed_To_Cardinal(distance, Options.Volume), variation, pan_value);
#endif
}


/***********************************************************************************************
 * Sound_Effect -- General purpose sound player.                                               *
 *                                                                                             *
 *    This is used for general purpose sound effects. These are sounds that occur outside      *
 *    of the game world. They do not have a corresponding game world location as their source. *
 *                                                                                             *
 * INPUT:   voc      -- The sound effect number to play.                                       *
 *                                                                                             *
 *          volume   -- The volume to assign to this sound effect.                             *
 *                                                                                             *
 * OUTPUT:  Returns with the sound handle (-1 if no sound was played).                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *   11/12/1994 JLB : Handles cache logic.                                                     *
 *   05/04/1995 JLB : Variation adjustments.                                                   *
 *=============================================================================================*/
int Sound_Effect(VocType voc, VolType volume, int variation, signed short pan_value)
{
	char name[_MAX_FNAME+_MAX_EXT];				// Working filename of sound effect.

	if (!Options.Volume || voc == VOC_NONE || !SoundOn || SampleType == SAMPLE_NONE) {
		return(-1);
	}

	/*
	**	Fetch a pointer to the sound effect data. Modify the sound as appropriate and desired.
	*/
	char const * ext = ".AUD";
	if (Special.IsJuvenile && SoundEffectName[voc].Where == IN_JUV) {
		ext = ".JUV";
	} else {
		if (SoundEffectName[voc].Where == IN_VAR) {

			/*
			**	For infantry, use a variation on the response. For vehicles, always
			**	use the vehicle response table.
			*/
			if (variation < 0) {
				if (ABS(variation) % 2) {
					ext = ".V00";
				} else {
					ext = ".V02";
				}
			} else {
				if (variation % 2) {
					ext = ".V01";
				} else {
					ext = ".V03";
				}
			}
		}
	}
	_makepath(name, NULL, NULL, SoundEffectName[voc].Name, ext);
	void const * ptr = MixFileClass::Retrieve(name);

	/*
	**	If the sound data pointer is not null, then presume that it is valid.
	*/
	if (ptr) {
		return(Play_Sample(ptr, Fixed_To_Cardinal(SoundEffectName[voc].Priority, (int)volume), (int)volume, pan_value));
	}
	return(-1);
}


/*
**	This elaborates all the EVA speech voices.
*/
char const * Speech[VOX_COUNT] =  {
	"ACCOM1",		//	mission accomplished
	"FAIL1",			//	your mission has failed
	"BLDG1",			//	unable to comply, building in progress
	"CONSTRU1",		//	construction complete
	"UNITREDY",		//	unit ready
	"NEWOPT1",		//	new construction options
	"DEPLOY1",		//	cannot deploy here
	"GDIDEAD1",		//	GDI unit destroyed
	"NODDEAD1",		//	Nod unit destroyed
	"CIVDEAD1",		//	civilian killed
//	"EVAYES1",		//	affirmative
//	"EVANO1",		//	negative
//	"UPUNIT1",		//	upgrade complete, new unit available
//	"UPSTRUC1",		//	upgrade complete, new structure available
	"NOCASH1",		//	insufficient funds
	"BATLCON1",		//	battle control terminated
	"REINFOR1",		//	reinforcements have arrived
	"CANCEL1",		//	canceled
	"BLDGING1",		//	building
	"LOPOWER1",		//	low power
	"NOPOWER1",		//	insufficient power
	"MOCASH1",		//	need more funds
	"BASEATK1",		//	our base is under attack
	"INCOME1",		//	incoming missile
	"ENEMYA",		//	enemy planes approaching
	"NUKE1",			//	nuclear warhead approaching - VOX_INCOMING_NUKE
//	"RADOK1",		//	radiation levels are acceptable
//	"RADFATL1",		//	radiation levels are fatal
	"NOBUILD1",		//	unable to build more
	"PRIBLDG1",		//	primary building selected
//	"REPDONE1",		//	repairs completed
	"NODCAPT1",		//	Nod building captured
	"GDICAPT1",		//	GDI building captured
//	"SOLD1",			//	structure sold
	"IONCHRG1",		//	ion cannon charging
	"IONREDY1",		//	ion cannon ready
	"NUKAVAIL",		//	nuclear weapon available
	"NUKLNCH1",		//	nuclear weapon launched - VOX_NUKE_LAUNCHED
	"UNITLOST",		// unit lost
	"STRCLOST",		// structure lost
	"NEEDHARV",		//	need harvester
	"SELECT1",		// select target
	"AIRREDY1",		// airstrike ready
	"NOREDY1",		//	not ready
	"TRANSSEE",		// Nod transport sighted
	"TRANLOAD",		// Nod transport loaded
	"ENMYAPP1",		//	enemy approaching
	"SILOS1",		//	silos needed
	"ONHOLD1",		//	on hold
	"REPAIR1",		//	repairing
	"ESTRUCX",		//	enemy structure destroyed
	"GSTRUC1",		//	GDI structure destroyed
	"NSTRUC1",		//	NOD structure destroyed
	"ENMYUNIT",		// Enemy unit destroyed
//	"GUKILL1",		//	gold unit destroyed
//	"GSTRUD1",		//	gold structure destroyed
//	"GONLINE1",		//	gold player online
//	"GLEFT1",		//	gold player has departed
//	"GOLDKILT",		//	gold player destroyed
//	"GOLDWIN",		//	gold player is victorious
//	"RUKILL1",		//	red unit destroyed
//	"RSTRUD1",		//	red structure destroyed
//	"RONLINE1",		//	red player online
//	"RLEFT1",		//	red player has departed
//	"REDKILT",		//	red player destroyed
//	"REDWIN",		//	red player is victorious
//	"GYUKILL1",		//	grey unit destroyed
//	"GYSTRUD1",		//	grey structure destroyed
//	"GYONLINE",		//	grey player online
//	"GYLEFT1",		//	grey player has departed
//	"GREYKILT",		//	grey player destroyed
//	"GREYWIN",		//	grey player is victorious
//	"OUKILL1",		//	orange unit destroyed
//	"OSTRUD1",		//	orange structure destroyed
//	"OONLINE1",		//	orange player online
//	"OLEFT1",		//	orange player has departed
//	"ORANKILT",		//	orange player destroyed
//	"ORANWIN",		//	orange player is victorious
//	"GNUKILL1",		//	green unit destroyed
//	"GNSTRUD1",		//	green structure destroyed
//	"GNONLINE",		//	green player online
//	"GNLEFT1",		//	green player has departed
//	"GRENKILT",		//	green player destroyed
//	"GRENWIN",		//	green player is victorious
//	"BUKILL1",		//	blue unit destroyed
//	"BSTRUD1",		//	blue structure destroyed
//	"BONLINE1",		//	blue player online
//	"BLEFT1",		//	blue player has departed
//	"BLUEKILT",		//	blue player destroyed
//	"BLUEWIN"		//	blue player is victorious
};
static VoxType CurrentVoice = VOX_NONE;


/***********************************************************************************************
 * Speak -- Computer speaks to the player.                                                     *
 *                                                                                             *
 *    This routine is used to have the game computer (EVA) speak to the player.                *
 *                                                                                             *
 * INPUT:   voice -- The voice number to speak (see defines.h).                                *
 *                                                                                             *
 * OUTPUT:  Returns with the handle of the playing speech (-1 if no voice started).            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/12/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Speak(VoxType voice, HouseClass *house, COORDINATE coord)
{
	// MBL 02.22.2019
	if (voice == VOX_NONE)
	{
		return;
	}

	//
	// Intercept speech calls. ST - 2/20/2019 3:37PM
	//
	// On_Speech((int)voice); // MBL 02.06.2020
	On_Speech((int)voice, house);
	if (coord) {
		On_Ping(house, coord);
	}

#if (0)	
	if (Options.Volume && SampleType != 0 && voice != VOX_NONE && voice != SpeakQueue && voice != CurrentVoice && SpeakQueue == VOX_NONE) {
		SpeakQueue = voice;
	}
#endif
}


/***********************************************************************************************
 * Speak_AI -- Handles starting the EVA voices.                                                *
 *                                                                                             *
 *    This starts the EVA voice talking as well. If there is any speech request in the queue,  *
 *    it will be started when the current voice is finished. Call this routine as often as     *
 *    possible (once per game tick is sufficient).                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Speak_AI(void)
{
// MBL 06.17.2019 KO
#if 0
	static VoxType _last = VOX_NONE;
	if (SampleType == 0) return;

	if (!Is_Sample_Playing(SpeechBuffer)) {
		CurrentVoice = VOX_NONE;
		if (SpeakQueue != VOX_NONE) {
			if (SpeakQueue != _last) {
				char name[_MAX_FNAME+_MAX_EXT];

				_makepath(name, NULL, NULL, Speech[SpeakQueue], ".AUD");
				if (CCFileClass(name).Read(SpeechBuffer, SPEECH_BUFFER_SIZE)) {
					Play_Sample(SpeechBuffer, 254, Options.Volume);
				}
				_last = SpeakQueue;
			} else {
				Play_Sample(SpeechBuffer, 254, Options.Volume);
			}
			SpeakQueue = VOX_NONE;
		}
	}
#endif
}


/***********************************************************************************************
 * Stop_Speaking -- Forces the EVA voice to stop talking.                                      *
 *                                                                                             *
 *    Use this routine to immediately stop the EVA voice from speaking. It also clears out     *
 *    the pending voice queue.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   12/27/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void Stop_Speaking(void)
{
	SpeakQueue = VOX_NONE;
	if (SampleType != 0) {
		Stop_Sample_Playing(SpeechBuffer);
	}
}


/***********************************************************************************************
 * Is_Speaking -- Checks to see if the eva voice is still playing.                             *
 *                                                                                             *
 *    Call this routine when the EVA voice being played needs to be checked. A typical use     *
 *    of this would be when some action needs to be delayed until the voice has finished --    *
 *    say the end of the game.                                                                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the EVA voice still playing?                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/12/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
bool Is_Speaking(void)
{
	Speak_AI();
	if (SampleType != 0 && (SpeakQueue != VOX_NONE || Is_Sample_Playing(SpeechBuffer))) {
		return(true);
	}
	return(false);
}