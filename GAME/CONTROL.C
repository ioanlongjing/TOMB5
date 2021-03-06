#include "CONTROL.H"

#include "3D_GEN.H"
#include "CD.H"
#if PSX_VERSION || PSXPC_VERSION
#include "COLLIDE_S.H"
#endif
#include "BOX.H"
#include "DELTAPAK.H"
#include "DEBRIS.H"
#include "DRAW.H"
#include "DRAWPHAS.H"
#include "EFFECTS.H"
#include "EFFECT2.H"
#if PC_VERSION
	#include "GAME.H"
#endif
#include "GAMEFLOW.H"
#if PSX_VERSION || PSXPC_VERSION
#include "GPU.H"
#endif
#include "HAIR.H"
#include "HEALTH.H"
#include "ITEMS.H"
#include "LARA.H"
#if PSX_VERSION || PSXPC_VERSION
#include "LOAD_LEV.H"
#endif
#include "LOT.H"
#include "MATHS.H"
#include "NEWINV2.H"
#include "PICKUP.H"
#include INPUT_H
#include "ROOMLOAD.H"
#if PSX_VERSION || PSXPC_VERSION
#include "PSOUTPUT.H"
#include "SETUP.H"
#endif
#include "SAVEGAME.H"
#include "SOUND.H"
#include "SPECIFIC.H"
#include "SPHERE.H"
#include "SPOTCAM.H"
#include "TOMB4FX.H"

#include <assert.h>
#include <string.h>

int flipeffect = -1;
int fliptimer;
unsigned char ShatterSounds[18][10] =
{
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_WOOD,  SFX_SMASH_WOOD,  SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_METAL, SFX_SMASH_METAL, SFX_SMASH_METAL, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_METAL, SFX_SMASH_METAL, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_METAL, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_METAL, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS },
	{ SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS, SFX_SMASH_GLASS }
};
unsigned char WeaponDelay;
unsigned char KeyTriggerActive;
unsigned short GlobalCounter;
char TriggerTimer;
int reset_flag;
short SlowMotion;
short SlowMoFrameCount = 16;
unsigned char InItemControlLoop;
short ItemNewRoomNo;
short SmashedMeshCount;
char richcutfrigflag;
int nRope;
char GetLaraOnLOS;
int NoInput;
int number_los_rooms;
int framecount;
struct ITEM_INFO* items;
int flip_status;
#if PC_VERSION
int flipmap[255];
int flip_stats[255];
#else
int flipmap[10];
int flip_stats[10];
#endif
int height_type;
int tiltxoff;
int tiltyoff;
unsigned long _CutSceneTriggered1;
unsigned long _CutSceneTriggered2;
unsigned long FmvSceneTriggered;
unsigned char CurrentAtmosphere;
unsigned char IsAtmospherePlaying;
char* OutsideRoomTable;
short* OutsideRoomOffsets;
short IsRoomOutsideNo;
short FXType;
int OnObject;
short* trigger_index;
char cd_flags[136];
unsigned char InGameCnt;
struct RAT_STRUCT* Rats;
struct BAT_STRUCT* Bats;
struct SPIDER_STRUCT* Spiders;
struct TWOGUN_INFO twogun[4];
int SetDebounce;
short WB_room;
struct ITEM_INFO* WB_item;
unsigned char HeavyTriggered;
struct MESH_INFO* SmashedMesh[16];
short SmashedMeshRoom[16];
struct PENDULUM CurrentPendulum;
char LaraDrawType;
char WeatherType;
char RoomDrawType;
struct PHD_VECTOR ClosestCoord;
int ClosestItem;
int ClosestDist;
short XSoff1;
short YSoff1;
short ZSoff1;
short XSoff2;
short YSoff2;
short ZSoff2;
short los_rooms[20];
char globoncuttrig;
short ItemNewRooms[256][2];
struct CHARDEF CharDef[106] =
{
	{ -0x52, 0x34, 4, 0xD, -0xB, 0, 0xB },
	{ 0x62, 0x3A, 7, 5, -0xA, 1, 5 },
	{ 0x52, 0x1A, 0xE, 0xC, -0xA, 1, 0xB },
	{ 0x4E, 0x26, 0xA, 0xE, -0xA, 1, 0xD },
	{ -0x2A, 0xD, 0xF, 0xC, -9, 2, 0xC },
	{ 0x28, 0x1A, 0xE, 0xC, -0xA, 1, 0xB },
	{ -0x62, 0x39, 5, 5, -0xB, 0, 4 },
	{ -0x34, 0x27, 6, 0x10, -0xC, 0, 0xD },
	{ 0x22, 0x28, 6, 0x10, -0xB, 0, 0xE },
	{ -0x48, 0x3B, 5, 5, -0xB, 0, 4 },
	{ 0x16, 0x28, 0xB, 0xB, -9, 2, 0xB },
	{ -0x4E, 0x3B, 5, 5, -2, 8, 0xC },
	{ 0x6A, 0x3C, 8, 3, -4, 6, 9 },
	{ 0x72, 0x3C, 5, 4, -2, 8, 0xB },
	{ -0x2C, 0x26, 9, 0xF, -0xC, 0, 0xC },
	{ 0x58, 0x31, 0xA, 0xA, -8, 3, 0xB },
	{ -0x38, 0x37, 6, 0xA, -8, 3, 0xB },
	{ 0x2E, 0x34, 9, 0xA, -8, 3, 0xB },
	{ 0x58, 0x26, 8, 0xB, -8, 3, 0xC },
	{ 0x3E, 0x28, 0xB, 0xB, -8, 3, 0xC },
	{ -0x72, 0x30, 9, 0xC, -9, 2, 0xC },
	{ -0x18, 0x32, 9, 0xB, -9, 2, 0xB },
	{ 0x78, 0x2F, 9, 0xC, -9, 2, 0xC },
	{ 0x16, 0x33, 9, 0xB, -9, 2, 0xB },
	{ 0x6E, 0x31, 9, 0xB, -8, 3, 0xC },
	{ -0x68, 0x39, 5, 9, -7, 4, 0xB },
	{ -0x78, 0x39, 5, 0xA, -7, 4, 0xC },
	{ -0x4E, 0x28, 0xC, 0xA, -8, 3, 0xB },
	{ -0x2E, 0x35, 0xB, 7, -7, 4, 9 },
	{ -0x10, 0x28, 0xC, 0xA, -8, 3, 0xB },
	{ 0xC, 0x27, 0xA, 0xD, -0xB, 0, 0xB },
	{ 0x42, 0xD, 0x10, 0xE, -0xA, 1, 0xD },
	{ -0x7E, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ -0x2A, 0x19, 0xD, 0xD, -0xB, 0, 0xB },
	{ -0x7C, 0x23, 0xB, 0xD, -0xB, 0, 0xB },
	{ 0, 0x1A, 0xD, 0xD, -0xB, 0, 0xB },
	{ 0xE, 0x1A, 0xD, 0xD, -0xB, 0, 0xB },
	{ 0x42, 0x1B, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x4A, 0x1B, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x38, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ -0x22, 0x36, 5, 0xD, -0xB, 0, 0xB },
	{ 0x38, 0x34, 5, 0x10, -0xB, 0, 0xE },
	{ -0x1A, 0xF, 0xD, 0xD, -0xB, 0, 0xB },
	{ -0x70, 0x23, 0xB, 0xD, -0xB, 0, 0xB },
	{ 0x30, 0xD, 0x12, 0xD, -0xB, 0, 0xB },
	{ -0x70, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ 0x36, 0x1A, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x38, 0x1A, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x10, 0, 0xE, 0xF, -0xB, 0, 0xD },
	{ -0x62, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ -0x64, 0x23, 0xB, 0xD, -0xB, 0, 0xB },
	{ -0x54, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ 0x62, 0xD, 0xF, 0xD, -0xB, 0, 0xB },
	{ 0x52, 0xD, 0xF, 0xD, -0xB, 0, 0xB },
	{ 0x18, 0xD, 0x17, 0xD, -0xB, 0, 0xB },
	{ -0x46, 0xD, 0xD, 0xE, -0xB, 0, 0xC },
	{ 0x72, 0xD, 0xF, 0xD, -0xB, 0, 0xB },
	{ -0x1C, 0x1C, 0xC, 0xD, -0xB, 0, 0xB },
	{ 0x3E, 0x3C, 6, 4, -4, 6, 9 },
	{ -8, 0x3B, 6, 4, -4, 6, 9 },
	{ 0x58, 0x3B, 8, 4, -4, 6, 9 },
	{ -0x72, 0x3C, 7, 3, -3, 7, 9 },
	{ 0x78, 0x3B, 8, 4, -4, 6, 9 },
	{ -0xE, 0x3B, 5, 5, -0xB, 0, 4 },
	{ 0x62, 0x31, 0xB, 9, -7, 4, 0xB },
	{ 0x60, 0x23, 0xB, 0xE, -0xC, 0, 0xB },
	{ 0x48, 0x34, 9, 9, -7, 4, 0xB },
	{ 0, 0x27, 0xB, 0xC, -0xA, 1, 0xB },
	{ -0x5C, 0x34, 9, 9, -7, 4, 0xB },
	{ -0x58, 0x26, 0xA, 0xE, -0xC, 0, 0xB },
	{ 0x78, 0x23, 0xC, 0xC, -7, 4, 0xE },
	{ 0x6C, 0x23, 0xB, 0xE, -0xC, 0, 0xB },
	{ -0x3E, 0x1B, 6, 0xC, -0xA, 1, 0xB },
	{ 0x28, 0x33, 6, 0x10, -0xA, 1, 0xE },
	{ 0x1C, 0x1A, 0xC, 0xE, -0xC, 0, 0xB },
	{ 0x52, 0x34, 6, 0xD, -0xB, 0, 0xB },
	{ 0x60, 0x1A, 0x12, 9, -7, 4, 0xB },
	{ -0x68, 0x30, 0xC, 9, -7, 4, 0xB },
	{ 0x3E, 0x33, 0xA, 9, -7, 4, 0xB },
	{ -0xC, 0xF, 0xB, 0xD, -7, 4, 0xE },
	{ 0x34, 0x27, 0xA, 0xD, -7, 4, 0xE },
	{ 0xA, 0x34, 0xA, 9, -7, 4, 0xB },
	{ -0x42, 0x34, 9, 9, -7, 4, 0xB },
	{ 0, 0x33, 9, 0xB, -9, 2, 0xB },
	{ -0x4E, 0x32, 0xB, 9, -7, 4, 0xB },
	{ -0x7E, 0x30, 0xC, 9, -7, 4, 0xB },
	{ -0x7C, 0x1A, 0x12, 9, -7, 4, 0xB },
	{ -0xE, 0x32, 0xB, 9, -7, 4, 0xB },
	{ 0x28, 0x26, 0xB, 0xD, -7, 4, 0xE },
	{ -0x18, 0x29, 8, 9, -7, 4, 0xB },
	{ -0x22, 0x29, 9, 0xD, -7, 4, 0xE },
	{ -0x7E, 0x39, 6, 9, -7, 4, 0xB },
	{ -0x3E, 0x27, 0xA, 0xD, -0xA, 1, 0xC },
	{ 0x20, 0x38, 5, 0xC, -0xA, 1, 0xB },
	{ 0, 0xD, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x40, 0, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x58, 0, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x28, 0, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x6A, 0x1A, 0x12, 9, -8, 6, 0xB },
	{ -0x58, 0x1A, 0xD, 0xC, -9, 6, 0xB },
	{ 0x72, 0x1A, 0x12, 9, -8, 6, 0xB },
	{ -0x10, 0x1C, 0xD, 0xC, -9, 6, 0xB },
	{ 0, 0, 0x29, 0xD, -0xA, 6, 0xB },
	{ 0x54, 0, 0x29, 0xD, -0xA, 6, 0xB },
	{ 0x2A, 0, 0x29, 0xD, -0xA, 6, 0xB },
	{ 0x7E, 0, 0x29, 0xD, -0xA, 6, 0xB }
};

char byte_A3660;

long ControlPhase(long nframes, int demo_mode)//1D538(<), 1D6CC(<) //DO NOT TOUCH (PSX/PSXPC)
{
#if PC_VERSION
	S_Warn("[ControlPhase] - Unimplemented!\n");
#else
	short item_num;
	short nex;

	//s0 = a0
	//s6 = a1

	if (SlowMotion == 0)
	{
		//loc_1D5B4
		if (SlowMoFrameCount > 16)
		{
			//loc_1D5C8
			SlowMoFrameCount--;
		}
	}
	else
	{
		SlowMotion--;
		if (SlowMoFrameCount > 39)
		{
			//loc_1D5C8
			SlowMoFrameCount--;
		}
	}

	//loc_1D5CC
	while (nframes < SlowMoFrameCount / 8)
	{
#if PSX_VERSION
		VSync(0);
#endif
		nframes++;
	}

	//loc_1D60C
	nframes = 2;
	GnLastFrameCount = 0;

	//loc_1D618
	RegeneratePickups();

	if (nframes > 10)
	{
		nframes = 10;
	}//loc_1D630


	if (bTrackCamInit)
	{
		bUseSpotCam = 0;
	}

	//loc_1D64C
	SetDebounce = 1;
	framecount += nframes;

	if (framecount <= 0)
	{
		//loc_1E3B8
		return 0;
	}

	if (GLOBAL_enterinventory != -1)
	{
		return 0;
	}

	//s4 = -1
	//s3 = 1

	//loc_1D684
	while (GLOBAL_enterinventory == -1)
	{
		GlobalCounter++;
		UpdateSky();
		S_UpdateInput();

		if (bDisableLaraControl)
		{
			if (gfCurrentLevel != LVL5_TITLE)
			{
				dbinput = 0;
			}

			//loc_1D6D4
			input &= IN_LOOK;
		}

		//loc_1D6EC
		if (cutseq_trig != 0)
		{
			input = IN_NONE;
		}

		//loc_1D708
		SetDebounce = 0;
		if (gfLevelComplete != 0)
		{
			return 3;
		}

		if (reset_flag != 0)
		{
			reset_flag = 0;
			return 1;
		}

		if (lara.death_count > 90)//loc_1D708, loc_1D89C
		{
			//loc_1D5A0
			reset_flag = 0;
			return S_Death();
		}

		if (demo_mode)
		{
			if (PadConnected && !ScreenFading)
			{
				PrintString(256, 230, 2, &gfStringWad[gfStringOffset[STR_DEMO_MODE]], 0);
			}

			//loc_1D7A0
			if (input == -1)
			{
				input = IN_NONE;
				Motors[0] = 0;
				Motors[1] = 0;
			}
		}//loc_1D7D4
		else
		{
			if (gfGameMode != 1 && Gameflow->CheatEnabled)
			{
				if (input == IN_NONE)
				{
					if (Gameflow->InputTimeout < NoInput++)
					{
						return 1;
					}
				}//1D844
				else
				{
					NoInput = 0;
				}
			}
		}

		//loc_1D848, loc_1D9DC 

#if DISC_VERSION///@TODO
	//loc_1D9DC
		if (input == IN_NONE)
		{
			//lbu	$v0, byte_A335A
			//lbu	$a0, 0x14DC($gp)
			//bnez	$v0, loc_1DA5C
			//andi	$v0, $a0, 0xFF
			//lbu	$v0, byte_A2827
			//nop
			//bnez	$v0, loc_1DA5C
			//andi	$v0, $a0, 0xFF
			//sltiu	$v0, $a0, 4
			//bnez	$v0, loc_1DA68
			//addiu	$v0, $a0, 1
			//addiu	$v0, $s2, 0x71E8
			//lh	$v1, 0x1A($v0)
			//nop
			//bnez	$v1, loc_1DA5C
			//andi	$v0, $a0, 0xFF
		}
		//loc_1DA3C:
		//sb	$zero, byte_800A601D
		// sb	$zero, byte_800A601C//motors maybe
		//jal	sub_62190
		//nop
		//j	loc_1E5D0
		// move	$v0, $zero
#endif

	//1D848
		if (InGameCnt < 4)
		{
			InGameCnt++;
		}

		//loc_1D860
		//a1 = input
#if 0//TODO highly mangled branching
		if (!(input & IN_LOOK) && SniperCamActive && bUseSpotCam && bTrackCamInit)
		{
			//loc_1D9D0
		}
		//v0 = 2
		//a0 = lara_item;
		//v1 = lara_item->current_anim_state;
		//v1 = lara (lara_info);
		if (lara_item->current_anim_state == 2 && lara_item->anim_number != 0x67)
		{
			//loc_1D8E0
			if (lara_item->pos.y_pos & 0x800 && !(input & 0x2000) && lara_item->anim_number == 0xDE && lara_item->goal_anim_state == 0x47)
			{
				//loc_1D920
			}//loc_1D9D0
			else
			{
			}

		}//loc_1D920

		//^TODO!!!
	}
	else
	{
		//loc_1D9D0 //***TODO
	}

	if (BinocularRange != 0)
	{
		if (LaserSight != 0)
		{


		}//loc_1DAC0
	}//loc_1DA80
#endif

	//loc_1DAD0
	if (BinocularRange != 0)
	{
		if (LaserSight != 0)
		{
			//loc_1DB28
			if (gfLevelFlags & GF_LVOP_TRAIN)
			{
				InfraRed = 0;
			}
			else
			{
				InfraRed = 1;
			}
		}
		else
		{
			if (gfLevelFlags & GF_LVOP_TRAIN && inputBusy & 0x40)
			{
				InfraRed = 1;
			}
			else
			{
				InfraRed = 0;
			}
		}
	}
	else
	{
		InfraRed = 0;
	}

	//loc_1DB44 
	//infr skp stre
	ClearDynamics();
	ClearFires();

	item_num = next_item_active;
	GotLaraSpheres = 0;
	InItemControlLoop = 1;

	if (item_num != -1)
	{
		//loc_1DB80
		while (items[item_num].next_active != -1)
		{
			if (items[item_num].after_death > 127)
			{
				KillItem(item_num);
			}
			else
			{
				//loc_1DBB4
				if (objects[items[item_num].object_number].control != NULL)
				{
					objects[items[item_num].object_number].control(item_num);
				}
			}

			item_num++;
		}

	}//loc_1DBE8, 1DDF4

	InItemControlLoop = 0;
	KillMoveItems();
	InItemControlLoop = 1;
	nex = next_fx_active;

	while (nex != -1)
	{
		nex = effects[next_fx_active].next_active;
		if (objects[effects[nex].object_number].control != NULL)
		{
			objects[effects[nex].object_number].control(nex);
		}
	}

	//loc_1DC60
	InItemControlLoop = 0;
	KillMoveEffects();

	if (KillEverythingFlag != 0)
	{
		KillEverything();
	}

	//loc_1DC88
	if (SmokeCountL != 0)
	{
		SmokeCountL--;
	}

	//loc_1DCA4
	if (SmokeCountR != 0)
	{
		SmokeCountR--;
	}

	//loc_1DCC0
	if (SplashCount != 0)
	{
		SplashCount--;
	}

	//loc_1DCDC
	if (WeaponDelay != 0)
	{
		WeaponDelay--;
	}

	//loc_1DCF0
	if (lara.burn && !(wibble & 0x7F))
	{
		AlertNearbyGuards(lara_item);
		lara.burn = 0;

	}//loc_1DD40  // *VERIFIED TILL HERE, FIXME below looks very mangled?

	XSoff1 += 150;
	YSoff1 += 230;
	ZSoff1 += 660;

	XSoff2 += 270;
	YSoff2 += 440;
	ZSoff2 += 160;

	////a000 = lara;

	if (lara.poisoned)
	{
		if (GLOBAL_playing_cutseq == 0)
		{
			//assert(0);
		}//loc_1DE90

	}//loc_1DE84

	//a1 = lara

	//loc_1DE90
	lara.skelebob = 0;
	InItemControlLoop = 1;

	if (GLOBAL_playing_cutseq == 0 && gfGameMode == 0)
	{
		//assert(0);
#if 0
		lui	$v1, 0xFFFE
			li	$v1, 0xFFFEFFFF
			lw	$v0, 0x44($a1)
			move	$a0, $zero
			and	$v0, $v1
			jal	sub_4A838
			sw	$v0, 0x44($a1)
			lbu	$v1, 0x1C58($gp)
			li	$v0, 5
			bne	$v1, $v0, loc_1DEF4
			addiu	$v0, $s5, -0x2240
			lw	$v1, 0x18($v0)
			nop
			lw	$a0, 0($v1)
			nop
			jalr	$a0
			nop
#endif
	}
	//loc_1DEF4
	InItemControlLoop = 0;

	KillMoveItems();

	if ((gfLevelFlags & GF_LVOP_TRAIN) && bUseSpotCam == 0)
	{
		//v0 = lara_item;
		//assert(0);
	}

	//loc_1DF7C
	if (GLOBAL_inventoryitemchosen != -1)
	{
		SayNo();
		GLOBAL_inventoryitemchosen = 1;
	}

	//loc_1DFA0
	if (GLOBAL_playing_cutseq == 0)//TODO
	{
		if (LaraDrawType != 5)
		{
			HairControl(0, 0, 0);

			if ((gfLevelFlags) & GF_LVOP_YOUNG_LARA)
			{
				HairControl(0, 1, 2);
			}
		}
		//loc_1DFF4
		if (GLOBAL_playing_cutseq == 0)//redudant
		{
			if (bUseSpotCam != 0)
			{
				CalculateSpotCams();
				//j loc_1E054///@FIXME illegal jump?
			}//loc_1E02C
			CalculateCamera();
		}//loc_1E044

	}//loc_1E044

	camera.type = CINEMATIC_CAMERA;
	CalculateCamera();

	//loc_1E054
	CamRot.vy = mGetAngle(camera.pos.x, camera.pos.z, camera.target.x, camera.target.z) / 16;

	wibble = wibble + 4 & 0xFC;

	TriggerLaraDrips();

	if (SmashedMeshCount != 0)//LOOP
	{
		//TODO
		//SmashedMeshCount = v000000;
		//v0 = CamRot.vy;
	}

	//loc_1E15C
#if 0
	UpdateSparks();
	//addiu	$s0, $s5, -0x2240
	UpdateFireSparks();
	UpdateSmokeSparks();
	UpdateBubbles();
	UpdateSplashes();
	UpdateDebris();
	UpdateBlood();
	UpdateDrips();
	UpdateGunShells();

#endif

	if (RelocPtr[9] != NULL)
	{
		//unsigned long* v0 = (unsigned long*)RelocPtr[9];
		//jalr v0[5];
	}

	//loc_1E1C4
	if (RelocPtr[21] != NULL)
	{
		//unsigned long* v0 = (unsigned long*)RelocPtr[21];
		//jalr v0[1];
	}

	//loc_1E1E4
	if (RelocPtr[32] != NULL)
	{
		//unsigned long* v0 = (unsigned long*)RelocPtr[32];
		//jalr v0[5];
	}

	//loc_1E204
	//UpdateShockwaves();

	if (RelocPtr[35] != NULL)
	{
		//unsigned long* v0 = (unsigned long*)RelocPtr[35];
		//jalr v0[1];
	}

	//loc_1E22C
	if (RelocPtr[7] != NULL)
	{
		//unsigned long* v0 = (unsigned long*)RelocPtr[7];
		//jalr v0[2];
	}

	//loc_1E24C
	AnimateWaterfalls();
	//UpdatePulseColour();

	if (gfCurrentLevel == LVL5_SINKING_SUBMARINE)
	{
		if (RelocPtr[48] != NULL)
		{
			//unsigned long* v0 = (unsigned long*)RelocPtr[48];
			//jalr v0[0];
		}
	}

	//loc_1E288
	SoundEffects();
	health_bar_timer--;

	if (gfGameMode == 0)
	{
		GameTimer++;

		if (savegame.Level.Timer != 0 && GLOBAL_playing_cutseq == 0)
		{
			savegame.Level.Timer++;
		}
	}

	//loc_1E2F0
	VibratePad();

	if (Motors[1] == 0 && camera.bounce)
	{
		//TODO

	}

	//loc_1E384
	UpdateFadeClip();
	framecount -= 2;

	if (framecount <= 0)
	{
		break;
	}
	}
#endif

	//loc_1E3B8
return 0;
}


void KillMoveItems()//1D420(<), 1D5B4(<) (F)
{
	short nex;

	if (ItemNewRoomNo > 0)
	{
		for(nex = 0; nex < ItemNewRoomNo; nex++)
		{
			if (ItemNewRooms[nex][0] & 0x8000)
			{
				KillItem(ItemNewRooms[nex][0] & 0x7FFF);
			}
			else
			{
				ItemNewRoom(ItemNewRooms[nex][0], ItemNewRooms[nex][1]);
			}

		}
	}

	ItemNewRoomNo = 0;
}

void KillMoveEffects()//1D4AC(<), 1D640(<) (F) 
{
	short nex;

	if (ItemNewRoomNo > 0)
	{
		for (nex = 0; nex < ItemNewRoomNo; nex++)
		{
			if (ItemNewRooms[nex][0] & 0x8000)
			{
				KillEffect(ItemNewRooms[nex][0] & 0x7FFF);
			}
			else
			{
				EffectNewRoom(ItemNewRooms[nex][0], ItemNewRooms[nex][1]);
			}

		}
	}

	ItemNewRoomNo = 0;
}

void TestTriggers(short* data, int heavy, int HeavyFlags)//(F)
{
	globoncuttrig = 0;
	_TestTriggers(data, heavy, HeavyFlags);
	if (!globoncuttrig)
	{
		if (richcutfrigflag)
			richcutfrigflag = 0;
	}
}

long rand_1 = 0xD371F947;

long GetRandomControl()//5E9F0, 926F8 (F)
{
	rand_1 = (rand_1 * 0x41C64E6D) + 0x3039;
	return (rand_1 >> 16) & 0x7FFF;
}

void SeedRandomControl(long seed)//(F)
{
	rand_1 = seed;
}

long rand_2 = 0xD371F947;

long GetRandomDraw()//5EA18, 5F6F8 (F)
{
	rand_2 = (rand_2 * 0x41C64E6D) + 0x3039;
	return (rand_2 >> 16) * 0x7FFF;
}

void SeedRandomDraw(long seed)//(F)
{
	rand_2 = seed;
}

void ClearFires()//8B1C8(<), 8D20C(<) (F)
{
	int i;

	for (i = 0; i < 32; i++)
	{
		fires[i].on = 0;
	}
}

void AddFire(int x, int y, int z, char size, short room_num, short on)// (F)
{
	struct FIRE_LIST* fptr = &fires[0];

	while (fptr->on)
	{
		fptr++;
		if (fptr - fires >= 32)
			return;
	}

	if (on)
		fptr->on = on;
	else
		fptr->on = 1;

	fptr->x = x;
	fptr->y = y;
	fptr->z = z;
	fptr->size = size;
	fptr->room_number = room_num;
}

void ClearDynamics()//8B1EC(<), 8D230(<) (F)
{
	int i;

#if 0
	//Not sure why Core have only clearing 32 entries here.
	//Either bug or intentinal.
	for (i = 0; i < 64; i++)
#else
	for (i = 0; i < 32; i++)
#endif
	{
		dynamics[i].on = 0;
	}
}

int is_object_in_room(int roomnumber, int objnumber)// (F)
{
	short item_num = room[roomnumber].item_number;
	short nex;
	struct ITEM_INFO* item;

	for (nex = item_num; nex != -1; nex = item->next_item)
	{
		item = &items[nex];

		if (item->object_number == objnumber)
			return TRUE;
	}

	return FALSE;
}

void NeatAndTidyTriggerCutscene(int value, int timer)
{
	S_Warn("[NeatAndTidyTriggerCutscene] - Unimplemented!\n");
}

int CheckCutPlayed(int num)//20E34(<), 21040(<) (F)
{
	if (num < 32)
		return _CutSceneTriggered1 & (1 << num);
	else
		return _CutSceneTriggered2 & (1 << (num - 32));
}

void SetCutNotPlayed(int num)//20DEC(<), 20FF8(<) (F)
{
	if (num < 32)
		_CutSceneTriggered1 &= ~(1 << num);
	else
		_CutSceneTriggered2 &= ~(1 << (num - 32));
}

void SetCutPlayed(int num)//20DA0(<), 20FAC(<) (F)
{
	if (num < 32)
		_CutSceneTriggered1 |= 1 << num;
	else
		_CutSceneTriggered2 |= 1 << (num - 32);
}

void InitCutPlayed()//20D90, 20F9C (F)
{
	_CutSceneTriggered1 = 0;
	_CutSceneTriggered2 = 0;
}

void ResetGuards()
{
	S_Warn("[ResetGuards] - Unimplemented!\n");
}

void InterpolateAngle(short dest, short* src, short* diff, short speed)
{
	S_Warn("[InterpolateAngle] - Unimplemented!\n");
}

int CheckGuardOnTrigger()
{
	S_Warn("[CheckGuardOnTrigger] - Unimplemented!\n");
	return 0;
}

int ExplodeItemNode(struct ITEM_INFO* item, int Node, int NoXZVel, long bits)//207DC(<), 209F0(<) (F)
{
	struct object_info* object;
	short* meshp;
	short num;

	if (item->mesh_bits & (1 << Node))
	{
		if (item->object_number != SWITCH_TYPE7 || gfCurrentLevel != LVL5_SINKING_SUBMARINE && gfCurrentLevel != LVL5_BASE)
		{
			num = bits;
			if (bits == 256)
				num = -64;
		}
		else
		{
			SoundEffect(SFX_SMASH_METAL, &item->pos, 0);
			num = bits;
		}
		GetSpheres(item, Slist, 3);
		object = &objects[item->object_number];
		ShatterItem.YRot = item->pos.y_rot;
		meshp = meshes[object->mesh_index + 2 * Node];
		ShatterItem.Bit = 1 << Node;
		ShatterItem.meshp = meshp;
		ShatterItem.Sphere.x = Slist[Node].x;
		ShatterItem.Sphere.y = Slist[Node].y;
		ShatterItem.Sphere.z = Slist[Node].z;
		ShatterItem.il = &item->il;
		ShatterItem.Flags = item->object_number != CROSSBOW_BOLT ? 0 : 0x400;
		ShatterObject(&ShatterItem, 0, num, item->room_number, NoXZVel);
		item->mesh_bits &= ~ShatterItem.Bit;
		return 1;
	}
	else
	{
		return 0;
	}
}

int GetTargetOnLOS(struct GAME_VECTOR* src, struct GAME_VECTOR* dest, int DrawTarget, int firing)
{
	S_Warn("[GetTargetOnLOS] - Unimplemented!\n");
	return 0;
}

void FireCrossBowFromLaserSight(struct GAME_VECTOR* src, struct GAME_VECTOR* target)
{
	S_Warn("[FireCrossBowFromLaserSight] - Unimplemented!\n");
}

void TriggerNormalCDTrack(short value, short flags, short type)// (F)
{
	int code;

	if (value == 117 || value == 118 || value == 121 || (value >= 123 && value <= 130))
	{
		if (CurrentAtmosphere != value)
		{
			CurrentAtmosphere = value;

			if (IsAtmospherePlaying)
				S_CDPlay(value, 1);
		}
	}
	else
	{
		code = (flags >> 8) & 0x3F;

		if ((code & cd_flags[value]) != code)
		{
			cd_flags[value] |= code;
			S_CDPlay(value, 0);
			IsAtmospherePlaying = 0;
		}
	}
}

void TriggerCDTrack(short value, short flags, short type)// (F)
{
	if (value < 136)
		TriggerNormalCDTrack(value, flags, type);
}

void RemoveRoomFlipItems(struct room_info* r)//1F938(<), 1FB4C(<) (F)
{
	short item_num;

	for (item_num = r->item_number; item_num != -1; item_num = items[item_num].next_item)
	{
		if (items[item_num].flags & IFLAG_INVISIBLE)
		{
			if (objects[items[item_num].object_number].intelligent)
			{
				if (items[item_num].hit_points <= 0 && items[item_num].hit_points != 0xC000)
				{
					KillItem(item_num);
				}
			}
		}
	}
}

void FlipMap(int FlipNumber)// (F)
{
	struct room_info* r = room;
	int i;
	for (i = 0; i < number_rooms; i++, r++)
	{
		if (r->flipped_room >= 0 && r->FlipNumber == FlipNumber)
		{
			struct room_info temp;
			struct room_info* flipped = &room[r->flipped_room];
			RemoveRoomFlipItems(r);
			memcpy(&temp, r, sizeof(struct room_info));
			memcpy(r, flipped, sizeof(struct room_info));
			memcpy(flipped, &temp, sizeof(struct room_info));
			r->flipped_room = flipped->flipped_room;
			flipped->flipped_room = -1;
			r->item_number = flipped->item_number;
			r->fx_number = flipped->fx_number;
			AddRoomFlipItems(r);
		}
	}
	flip_stats[FlipNumber] = flip_stats[FlipNumber] == 0;
	flip_status = flip_stats[FlipNumber] == 0;
	{
		struct creature_info* cinfo = baddie_slots;
		int slot;
		for (slot = 0; slot < 6; slot++, cinfo++)
		{
			cinfo->LOT.target_box = 0x7FF;
		}
	}
}

void _TestTriggers(short* data, int heavy, int HeavyFlags)
{
	S_Warn("[_TestTriggers] - Unimplemented!\n");
}

void RefreshCamera(short type, short* data)
{
	S_Warn("[RefreshCamera] - Unimplemented!\n");
}

long GetWaterHeight(long x, long y, long z, short room_number)
{
	S_Warn("[GetWaterHeight] - Unimplemented!\n");
	return 0;
}

void AlterFloorHeight(struct ITEM_INFO* item, int height)
{
	S_Warn("[AlterFloorHeight] - Unimplemented!\n");
}

short GetHeight(struct FLOOR_INFO* floor, int x, int y, int z)
{
	S_Warn("[GetHeight] - Unimplemented!\n");
	return 0;
}

struct FLOOR_INFO* GetFloor(int x, int y, int z, short* room_number)//78954(<), 7A998(<) (F)
{
	struct room_info* r;
	struct FLOOR_INFO* floor;
	int y_floor, x_floor, next_room;
	int tmp;

	for (r = &room[*room_number]; ; r = &room[next_room])
	{
		x_floor = (z - r->z) >> 10;
		y_floor = (x - r->x) >> 10;

		if (x_floor <= 0)
		{
			x_floor = 0;
			if (y_floor < 1)
			{
				y_floor = 1;
			}
			else if (y_floor > r->y_size - 2)
			{
				y_floor = r->y_size - 2;
			}
		}
		else if (x_floor >= r->x_size - 1)
		{
			x_floor = r->x_size - 1;
			if (y_floor < 1)
			{
				y_floor = 1;
			}
			else if (y_floor > r->y_size - 2)
			{
				y_floor = r->y_size - 2;
			}
		}
		else if (y_floor < 0)
		{
			y_floor = 0;
		}
		else if (y_floor >= r->y_size)
		{
			y_floor = r->y_size - 1;
		}

		floor = &r->floor[x_floor + y_floor * r->x_size];
		next_room = GetDoor(floor);
		if (next_room == 255)
			break;
		*room_number = next_room;
	}

	if (y < floor->floor << 8)
	{
		if (y < floor->ceiling << 8 && floor->sky_room != -1)
		{
			do
			{
				tmp = CheckNoColCeilingTriangle(floor, x, z);
				if (tmp == 1 || tmp == -1 && y >= r->maxceiling)
					break;
				*room_number = floor->sky_room;
				r = &room[floor->sky_room];
				floor = &r->floor[((z - r->z) >> 10) + r->x_size * ((x - r->x) >> 10)];
				if (y >= floor->ceiling << 8)
					break;
			} while (floor->sky_room != 0xFF);
		}
	}
	else if (floor->pit_room != 0xFF)
	{
		while (1)
		{
			tmp = CheckNoColFloorTriangle(floor, x, z);
			if (tmp == 1 || tmp == -1 && y < r->minfloor)
				break;
			*room_number = floor->pit_room;
			r = &room[floor->pit_room];
			tmp = ((z - r->z) >> 10) + r->x_size * ((x - r->x) >> 10);
			/*if (abs(tmp) > 1048576)
			{
				S_Warn("[GetFloor] - sector num too big -> probably room array not initialized\n");
				S_Warn("[GetFloor] - returning or the vc runtime will shit brixes\n");
				return floor;
			}*/
			floor = &r->floor[tmp];
			if (y < r->floor[tmp].floor << 8)
				break;
			if (floor->pit_room == 0xFF)
				return &r->floor[tmp];
		}
	}

	return floor;
}

short GetCeiling(struct FLOOR_INFO* floor, int x, int y, int z)
{
	S_Warn("[GetCeiling] - Unimplemented!\n");
	return 0;
}

int TriggerActive(struct ITEM_INFO* item)// (F)
{
	if ((item->flags & IFLAG_ACTIVATION_MASK) == IFLAG_ACTIVATION_MASK)
	{
		if (item->timer)
		{
			if (item->timer <= 0)
			{
				if (item->timer < -1)
				{
					item->timer++;
					if (item->timer == -1)
						item->timer = 0;
				}
			}
			else
			{
				item->timer--;
				if (item->timer == 0)
					item->timer = -1;
			}

			if (item->timer < 0)
				return (~item->flags & IFLAG_REVERSE) == 0;
		}
	}
	else
	{
		return (~item->flags & IFLAG_REVERSE) == 0;
	}

	return (~item->flags & IFLAG_REVERSE) != 0;
}

void AddRoomFlipItems(struct room_info* r)//1FA0C(<), 1FC20(<) (F)
{
	short item_num;

	for (item_num = r->item_number; item_num != -1; item_num = items[item_num].next_item)
	{
		if (items[item_num].item_flags[1])
		{
			switch (items[item_num].object_number)
			{
			case RAISING_BLOCK1:
				AlterFloorHeight(&items[item_num], SECTOR(-1));
				break;

			case RAISING_BLOCK2:
				AlterFloorHeight(&items[item_num], SECTOR(-2));
				break;
			}
		}
	}
}

int IsRoomOutside(long x, long y, long z)//8EF00(<), 90F44(<) (F)
{
	short off, room_num;
	struct FLOOR_INFO* floor;
	short height, ceiling;
	struct room_info* r;

	if (x < 0 || z < 0)
		return -2;

	off = OutsideRoomOffsets[(z / 4096) + 27 * (x / 4096)];

	if (off == -1)
		return -2;

	if (off >= 0)
	{
		char* ptr = &OutsideRoomTable[off];
		

		if (*ptr == -1)
			return -2;

		while (TRUE)
		{
			r = &room[*ptr];
			if (y >= r->maxceiling && y <= r->minfloor)
			{
				if (z >= r->z + 1024 && z <= (r->x_size / 1024) + r->z - 1024)
				{
					if (x >= r->x + 1024 && x <= (r->y_size / 1024) + r->x - 1024)
						break;
				}
			}
			ptr++;

			if (*ptr == -1)
				return -2;
		}

		IsRoomOutsideNo = room_num = *ptr;
	}
	else
	{
		r = &room[off & 0x7fff];

		if (y < r->maxceiling || y > r->minfloor)
			return -2;

		if (z < r->z + 1024 || z > r->x_size / 1024 + r->z - 1024)
			return -2;

		if (x < r->x + 1024 || x > r->y_size / 1024 + r->x - 1024)
			return -2;

		IsRoomOutsideNo = room_num = off & 0x7fff;
	}

	floor = GetFloor(x, y, z, &room_num);

	height = GetHeight(floor, x, y, z);
	if (height == -32512 || y > height)
		return -2;

	ceiling = GetCeiling(floor, x, y, z);

	if (y >= ceiling)
		return (r->flags & RF_WIND_BLOWS_PONYTAIL || r->flags & RF_FILL_WATER) ? 1 : -3;

	return -2;
}

short GetDoor(struct FLOOR_INFO* floor)//787CC(<), 7A810(<) (F)
{
	int type, fixtype;
	short* data;

	if (!floor->index)
		return 255;

	type = floor_data[floor->index];
	data = &floor_data[floor->index + 1];

	fixtype = type & FD_MASK_FUNCTION;

	if (fixtype == TILT_TYPE
		|| fixtype == SPLIT1
		|| fixtype == SPLIT2
		|| fixtype == NOCOLF1B
		|| fixtype == NOCOLF1T
		|| fixtype == NOCOLF2B
		|| fixtype == NOCOLF2T)
	{
		if (type & FD_MASK_END_DATA)
			return 255;
		type = data[1];
		data += 2;
	}

	fixtype = type & FD_MASK_FUNCTION;

	if (fixtype == ROOF_TYPE
		|| fixtype == SPLIT3
		|| fixtype == SPLIT4
		|| fixtype == NOCOLC1B
		|| fixtype == NOCOLC1T
		|| fixtype == NOCOLC2B
		|| fixtype == NOCOLC2T)
	{
		if (type & FD_MASK_END_DATA)
			return 255;
		type = data[1];
		data += 2;
	}

	if ((type & FD_MASK_FUNCTION) == DOOR_TYPE)
		return *data;

	return 255;
}

int LOS(struct GAME_VECTOR* start, struct GAME_VECTOR* target)//79460(<), 7B4A4(<) (F)
{
	struct FLOOR_INFO* floor;
	int los1, los2;

	if ((abs(target->z - start->z)) > (abs(target->x - start->x)))
	{
		los1 = xLOS(start, target);
		los2 = zLOS(start, target);
	}
	else
	{
		los1 = zLOS(start, target);
		los2 = xLOS(start, target);
	}

	if (los2)
	{
		floor = GetFloor(target->x, target->y, target->z, &target->room_number);

		if (ClipTarget(start, target, floor) || los1 == 1 || los2 == 1)
			return 1;
	}

	return 0;
}

int xLOS(struct GAME_VECTOR* start, struct GAME_VECTOR* target)
{
	S_Warn("[xLOS] - Unimplemented!\n");
	return 0;
}

int zLOS(struct GAME_VECTOR* start, struct GAME_VECTOR* target)
{
	S_Warn("[zLOS] - Unimplemented!\n");
	return 0;
}

int CheckNoColCeilingTriangle(struct FLOOR_INFO* floor, int x, int z)// (F)
{
	short* fd = &floor_data[floor->index];
	short fixtype = *fd & FD_MASK_FUNCTION;
	int fix_x = x & 0x3FF;
	int fix_z = z & 0x3FF;

	if (floor->index == 0)
		return 0;

	if (fixtype == TILT_TYPE
		|| fixtype == SPLIT1
		|| fixtype == SPLIT2
		|| fixtype == NOCOLF1T
		|| fixtype == NOCOLF1B
		|| fixtype == NOCOLF2T
		|| fixtype == NOCOLF2B)
	{
		if (*fd & FD_MASK_END_DATA)
			return 0;

		fixtype = fd[2] & FD_MASK_FUNCTION;
	}

	switch (fixtype)
	{
	case NOCOLC1T:
		if (fix_x <= 1024 - fix_z)
			return -1;
		break;
	case NOCOLC1B:
		if (fix_x > 1024 - fix_z)
			return -1;
		break;
	case NOCOLC2T:
		if (fix_x <= fix_z)
			return -1;
		break;
	case NOCOLC2B:
		if (fix_x > fix_z)
			return -1;
		break;
	default:
		return 0;
	}

	return 1;
}

int CheckNoColFloorTriangle(struct FLOOR_INFO* floor, int x, int z)// (F)
{
	short fixtype = floor_data[floor->index] & FD_MASK_FUNCTION;
	int fix_x = x & 0x3FF;
	int fix_z = z & 0x3FF;

	if (floor->index == 0)
		return 0;

	switch (fixtype)
	{
	case NOCOLF1T:
		if (fix_x <= 1024 - fix_z)
			return -1;
		break;
	case NOCOLF1B:
		if (fix_x > 1024 - fix_z)
			return -1;
		break;
	case NOCOLF2T:
		if (fix_x <= fix_z)
			return -1;
		break;
	case NOCOLF2B:
		if (fix_x > fix_z)
			return -1;
		break;
	default:
		return 0;
	}

	return 1;
}

int ClipTarget(struct GAME_VECTOR* start, struct GAME_VECTOR* target, struct FLOOR_INFO* floor)
{
	S_Warn("[ClipTarget] - Unimplemented!\n");
	return 0;
}

void GetJointAbsPosition(struct ITEM_INFO* item, struct PHD_VECTOR* pos, int joint)
{
	S_Warn("[GetJointAbsPosition] - Unimplemented!\n");
}

int ObjectOnLOS2(struct GAME_VECTOR* start, struct GAME_VECTOR* target, struct PHD_VECTOR* a3, struct MESH_INFO** a4)
{
	S_Warn("[ObjectOnLOS2] - Unimplemented!\n");
	return 0;
}

int check_xray_machine_trigger()// (F)
{
	int i;
	for (i = 0; i < level_items; i++)
	{
		if (items[i].object_number == XRAY_CONTROLLER &&
			items[i].trigger_flags == 0 &&
			items[i].item_flags[0] == 666)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void AnimateItem(struct ITEM_INFO* item)
{
	S_Warn("[AnimateItem] - Unimplemented!\n");
}

void Straighten(struct ROPE_STRUCT* a1, struct PHD_VECTOR* a2, struct PHD_VECTOR* a3, long a4)
{
	S_Warn("[Straighten] - Unimplemented!\n");
}

void StraightenRope(struct ITEM_INFO* item)
{
	S_Warn("[StraightenRope] - Unimplemented!\n");
}