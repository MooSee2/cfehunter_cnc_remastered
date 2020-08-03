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

/* $Header:   F:\projects\c&c\vcs\code\audio.h_v   2.18   16 Oct 1995 16:45:34   JOE_BOSTIC  $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : AUDIO.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : June 21, 1994                                                *
 *                                                                                             *
 *                  Last Update : June 21, 1994   [JLB]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef AUDIO_H
#define AUDIO_H

#include	"memory.h"

class AudioClass {
		char const * Name;	// Name of audio asset.
		void const * Data;	// Loaded audio data.
		int Handle;				// Handle of asset (as it is playing).
		MemoryClass *Mem;		// Pointer to memory handler class.
		unsigned IsMIDI:1;	// Is this a midi file?

	public:
		AudioClass(void);
		AudioClass(char const *name, MemoryClass &mem);
		virtual ~AudioClass(void);

		bool Load(char const *name = 0);
		bool Free(void);
		bool Play(int volume = 0xFF);
		bool Stop(void);
		bool Pause(void);
		bool Resume(void);
		bool Set_Name(char const *name);
		bool Is_Playing(void) const;
		bool Is_Loaded(void) const;
		bool Is_MIDI(void) const;
};

inline AudioClass::AudioClass(void)
{
	Name = 0; 
	Data = 0; 
	Mem = 0;
	Handle = -1;
};

inline AudioClass::AudioClass(char const *name, MemoryClass &mem) 
{
	if (mem) {
		Mem = &mem;
	} else {
		Mem = &::Mem;		// Uses global default memory handler.
	}
	Name = strdup(name);
	Data = 0;
	Handle = -1;
};

inline AudioClass::~AudioClass(void)
{
	if (GameActive) {
		if (Name) free(Name);
		if (Data) Mem->Free(Data);
		Name = 0;
		Data = 0;
		Handle = -1;
	}
};






/***************************************************************************
**	Controls what special effects may occur on the sound effect.
*/
typedef enum {
	IN_NOVAR,			// No variation or alterations allowed.
	IN_JUV,				// Juvenile sound effect alternate option.
	IN_VAR,				// Infantry variance response modification.
} ContextType;


// static struct { // MBL 02.21.2019
// Had to name the struct for VS 2017 distributed build. ST - 4/10/2019 3:59PM
struct SoundEffectNameStruct {
	char const *Name;		// Digitized voice file name.
	int			Priority;	// Playback priority of this sample.
	ContextType	Where;		// In what game context does this sample exist.
} SoundEffectName[VOC_COUNT] = {

	/*
	**	Special voices (typically associated with the commando).
	*/
	{"BOMBIT1",		20, IN_NOVAR},		//	VOC_RAMBO_PRESENT		"I've got a present for	ya"
	{"CMON1",		20, IN_NOVAR},		//	VOC_RAMBO_CMON			"c'mon"
	{"GOTIT1",		20, IN_NOVAR},		//	VOC_RAMBO_UGOTIT		"you got it" *
	{"KEEPEM1",		20, IN_NOVAR},		//	VOC_RAMBO_COMIN		"keep 'em commin'"
	{"LAUGH1",		20, IN_NOVAR},		//	VOC_RAMBO_LAUGH		"hahaha"
	{"LEFTY1",		20, IN_NOVAR},		//	VOC_RAMBO_LEFTY		"that was left handed" *
	{"NOPRBLM1",	20, IN_NOVAR},		//	VOC_RAMBO_NOPROB		"no problem"
//	{"OHSH1",		20, IN_NOVAR},		//	VOC_RAMBO_OHSH			"oh shiiiiii...."
	{"ONIT1",		20, IN_NOVAR},		//	VOC_RAMBO_ONIT			"I'm on it"
	{"RAMYELL1",	20, IN_NOVAR},		//	VOC_RAMBO_YELL			"ahhhhhhh"
	{"ROKROLL1",	20, IN_NOVAR},		//	VOC_RAMBO_ROCK			"time to rock and roll"
	{"TUFFGUY1",	20, IN_NOVAR},		//	VOC_RAMBO_TUFF			"real tuff guy" *
	{"YEAH1",		20, IN_NOVAR},		//	VOC_RAMBO_YEA			"yea" *
	{"YES1",			20, IN_NOVAR},		//	VOC_RAMBO_YES			"yes" *
	{"YO1",			20, IN_NOVAR},		//	VOC_RAMBO_YO			"yo"

	/*
	**	Civilian voices (technicians too).
	*/
	{"GIRLOKAY",	20, IN_NOVAR},		//	VOC_GIRL_OKAY
	{"GIRLYEAH",	20, IN_NOVAR},		//	VOC_GIRL_YEAH
	{"GUYOKAY1",	20, IN_NOVAR},		//	VOC_GUY_OKAY
	{"GUYYEAH1",	20, IN_NOVAR},		//	VOC_GUY_YEAH

	/*
	**	Infantry and vehicle responses.
	*/
	{"2DANGR1",		10,	IN_VAR},	//	VOC_2DANGER			"negative, too dangerous"
	{"ACKNO",		10,	IN_VAR},	//	VOC_ACKNOWL			"acknowledged"
	{"AFFIRM1",		10,	IN_VAR},	//	VOC_AFFIRM			"affirmative"
	{"AWAIT1",		10,	IN_VAR},	//	VOC_AWAIT1			"awaiting orders"
//	{"BACKUP",		10,	IN_VAR},	// VOC_BACKUP			"send backup"
//	{"HELP",			10,	IN_VAR},	// VOC_HELP				"send help"
	{"MOVOUT1",		10,	IN_VAR},	//	VOC_MOVEOUT			"movin' out"
	{"NEGATV1",		10,	IN_VAR},	//	VOC_NEGATIVE		"negative"
	{"NOPROB",		10,	IN_VAR},	// VOC_NO_PROB			"not a problem"
	{"READY",		10,	IN_VAR},	// VOC_READY			"ready and waiting"
	{"REPORT1",		10,	IN_VAR},	//	VOC_REPORT			"reporting"
	{"RITAWAY",		10,	IN_VAR},	// VOC_RIGHT_AWAY		"right away sir"
	{"ROGER",		10,	IN_VAR},	// VOC_ROGER			"roger"
//	{"SIR1",			10,	IN_VAR},	//	VOC_SIR1				"sir?"
//	{"SQUAD1",		10,	IN_VAR},	//	VOC_SQUAD1			"squad reporting"
//	{"TARGET1",		10,	IN_VAR},	//	VOC_PRACTICE		"target practice"
	{"UGOTIT",		10,	IN_VAR},	// VOC_UGOTIT			"you got it"
	{"UNIT1",		10,	IN_VAR},	//	VOC_UNIT1			"unit reporting"
	{"VEHIC1",		10,	IN_VAR},	//	VOC_VEHIC1			"vehicle reporting"
	{"YESSIR1",		10,	IN_VAR},	//	VOC_YESSIR			"yes sir"

	/*
	**	Sound effects that have a juvenile counterpart.
	*/
	{"BAZOOK1", 	1,		IN_JUV},	// VOC_BAZOOKA			Gunfire
	{"BLEEP2", 		1,		IN_JUV},	// VOC_BLEEP			Clean metal bing
	{"BOMB1", 		1,		IN_JUV},	// VOC_BOMB1			Crunchy parachute bomb type explosion
	{"BUTTON", 		1,		IN_JUV},	// VOC_BUTTON			Dungeon Master button click
	{"COMCNTR1",	10,		IN_JUV},	// VOC_RADAR_ON		Elecronic static with beeps
	{"CONSTRU2",	10,		IN_JUV},	// VOC_CONSTRUCTION	construction sounds
	{"CRUMBLE", 	1,		IN_JUV},	// VOC_CRUMBLE			muffled crumble sound
	{"FLAMER2", 	4,		IN_JUV},	// VOC_FLAMER1			flame thrower
	{"GUN18",		4, 		IN_JUV},	// VOC_RIFLE			rifle shot
	{"GUN19",		4, 		IN_JUV},	// VOC_M60				machine gun burst -- 6 rounds
	{"GUN20",		4, 		IN_JUV},	// VOC_GUN20			bat hitting heavy metal door
	{"GUN5",		4, 		IN_JUV},	// VOC_M60A				medium machine gun burst
	{"GUN8",		4, 		IN_JUV},	// VOC_MINI				mini gun burst
	{"GUNCLIP1",	1, 		IN_JUV},	// VOC_RELOAD			gun clip reload
	{"HVYDOOR1", 	5,		IN_JUV},	// VOC_SLAM				metal plates slamming together
	{"HVYGUN10",	1,		IN_JUV},	//	VOC_HVYGUN10		loud sharp cannon
	{"ION1", 		1,		IN_JUV},	// VOC_ION_CANNON		partical beam
	{"MGUN11", 		1,		IN_JUV},	// VOC_MGUN11			alternate tripple burst
	{"MGUN2", 		1,		IN_JUV},	// VOC_MGUN2			M-16 tripple burst
	{"NUKEMISL",	1,		IN_JUV},	//	VOC_NUKE_FIRE		long missile sound
	{"NUKEXPLO",	1,		IN_JUV},	//	VOC_NUKE_EXPLODE	long but not loud explosion
	{"OBELRAY1",	1,		IN_JUV},	//	VOC_LASER			humming laser beam
	{"OBELPOWR",	1,		IN_JUV},	// VOC_LASER_POWER	warming-up sound of laser beam
	{"POWRDN1",		1,		IN_JUV},	//	VOC_RADAR_OFF		doom door slide
	{"RAMGUN2",		1,		IN_JUV},	//	VOC_SNIPER			silenced rifle fire
	{"ROCKET1",		1,		IN_JUV},	//	VOC_ROCKET1			rocket launch variation #1
	{"ROCKET2",		1,		IN_JUV},	//	VOC_ROCKET2			rocket launch variation #2
	{"SAMMOTR2",	1, 		IN_JUV},	// VOC_MOTOR			dentists drill
	{"SCOLD2",		1, 		IN_JUV},	// VOC_SCOLD			cannot perform action feedback tone
	{"SIDBAR1C", 	1,		IN_JUV},	// VOC_SIDEBAR_OPEN	xylophone clink
	{"SIDBAR2C", 	1,		IN_JUV},	// VOC_SIDEBAR_CLOSE	xylophone clink
	{"SQUISH2", 	1,		IN_JUV},	// VOC_SQUISH2			crushing infantry
	{"TNKFIRE2",	1, 		IN_JUV},	// VOC_TANK1			sharp tank fire with recoil
	{"TNKFIRE3",	1, 		IN_JUV},	// VOC_TANK2			sharp tank fire
	{"TNKFIRE4",	1, 		IN_JUV},	// VOC_TANK3			sharp tank fire
	{"TNKFIRE6",	1, 		IN_JUV},	// VOC_TANK4			big gun tank fire
	{"TONE15",		0, 		IN_JUV},	// VOC_UP				credits counting up
	{"TONE16",		0, 		IN_JUV},	// VOC_DOWN				credits counting down
	{"TONE2",		1, 		IN_JUV},	// VOC_TARGET			target sound
	{"TONE5",		10,		IN_JUV},	// VOC_SONAR			sonar echo
	{"TOSS",		1, 		IN_JUV},	// VOC_TOSS				air swish
	{"TRANS1",		1, 		IN_JUV},	// VOC_CLOAK			stealth tank
	{"TREEBRN1",	1, 		IN_JUV},	// VOC_BURN				burning crackle
	{"TURRFIR5",	1, 		IN_JUV},	// VOC_TURRET			muffled gunfire
	{"XPLOBIG4",	5,		IN_JUV},	// VOC_XPLOBIG4		very long muffled explosion
	{"XPLOBIG6",	5,		IN_JUV},	// VOC_XPLOBIG6		very long muffled explosion
	{"XPLOBIG7",	5,		IN_JUV},	// VOC_XPLOBIG7		very long muffled explosion
	{"XPLODE",		1,		IN_JUV},	// VOC_XPLODE		long soft muffled explosion
	{"XPLOS",		4, 		IN_JUV},	// VOC_XPLOS		short crunchy explosion
	{"XPLOSML2",	5,		IN_JUV},	// VOC_XPLOSML2		muffled mechanical explosion
	{"ORCALND", 	1,		IN_JUV},	// VOC_ORCALAND		Orca take off sound.
	{"ORCATO",  	1,		IN_JUV },	// VOC_ORCATO  		Orca landing sound.

	/*
	**	Generic sound effects (no variations).
	*/
	{"NUYELL1", 	10,	IN_NOVAR},	// VOC_SCREAM1			short infantry scream
	{"NUYELL3", 	10,	IN_NOVAR},	// VOC_SCREAM3			short infantry scream
	{"NUYELL4", 	10,	IN_NOVAR},	// VOC_SCREAM4			short infantry scream
	{"NUYELL5", 	10,	IN_NOVAR},	// VOC_SCREAM5			short infantry scream
	{"NUYELL6", 	10,	IN_NOVAR},	// VOC_SCREAM6			short infantry scream
	{"NUYELL7", 	10,	IN_NOVAR},	// VOC_SCREAM7			short infantry scream
	{"NUYELL10", 	10,	IN_NOVAR},	// VOC_SCREAM10		short infantry scream
	{"NUYELL11", 	10,	IN_NOVAR},	// VOC_SCREAM11		short infantry scream
	{"NUYELL12", 	10,	IN_NOVAR},	// VOC_SCREAM12		short infantry scream
	{"YELL1", 		1,		IN_NOVAR},	// VOC_YELL1			long infantry scream

	{"MYES1", 		10,	IN_NOVAR},	// VOC_YES				"Yes?"
	{"MCOMND1", 	10,	IN_NOVAR},	// VOC_COMMANDER		"Commander?"
	{"MHELLO1", 	10,	IN_NOVAR},	//	VOC_HELLO			"Hello?"
	{"MHMMM1", 		10,	IN_NOVAR},	//	VOC_HMMM				"Hmmm?"
//	{"MHASTE1", 	10,	IN_NOVAR},	//	VOC_PROCEED1		"I will proceed, post haste."
//	{"MONCE1", 		10,	IN_NOVAR},	//	VOC_PROCEED2		"I will proceed, at once."
//	{"MIMMD1", 		10,	IN_NOVAR},	//	VOC_PROCEED3		"I will proceed, immediately."
//	{"MPLAN1", 		10,	IN_NOVAR},	//	VOC_EXCELLENT1		"That is an excellent plan."
//	{"MPLAN2", 		10,	IN_NOVAR},	//	VOC_EXCELLENT2		"Yes, that is an excellent plan."
	{"MPLAN3", 		10,	IN_NOVAR},	//	VOC_EXCELLENT3		"A wonderful plan."
//	{"MACTION1", 	10,	IN_NOVAR},	//	VOC_EXCELLENT4		"Astounding plan of action commander."
//	{"MREMARK1", 	10,	IN_NOVAR},	// VOC_EXCELLENT5		"Remarkable contrivance."
	{"MCOURSE1", 	10,	IN_NOVAR},	// VOC_OF_COURSE		"Of course."
	{"MYESYES1", 	10,	IN_NOVAR},	// VOC_YESYES			"Yes yes yes."
	{"MTIBER1", 	10,	IN_NOVAR},	//	VOC_QUIP1			"Mind the Tiberium."
//	{"MMG1", 		10,	IN_NOVAR},	//	VOC_QUIP2			"A most remarkable  Metasequoia Glyptostroboides."
	{"MTHANKS1", 	10,	IN_NOVAR},	//	VOC_THANKS			"Thank you."

	{"CASHTURN", 	1,		IN_NOVAR},	//	VOC_CASHTURN		Sound of money being piled up.
	{"BLEEP2", 		10,	IN_NOVAR},	//	VOC_BLEEPY3			Clean computer bleep sound.
	{"DINOMOUT", 	10,	IN_NOVAR},	//	VOC_DINOMOUT		Movin' out in dino-speak.
	{"DINOYES", 	10,	IN_NOVAR},	//	VOC_DINOYES			Yes Sir in dino-speak.
	{"DINOATK1", 	10,	IN_NOVAR},	//	VOC_DINOATK1		Dino attack sound.
	{"DINODIE1", 	10,	IN_NOVAR},	//	VOC_DINODIE1		Dino die sound.

};

#endif