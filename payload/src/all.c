#include "global.h"
#include "gflib.h"
#include "all.h"
#include "libpmagb/agb_rom.h"
#include "unk_200E344.h"
#include "unk_200C5DC.h"
#include "libpmagb/save.h"
#include "gflib/text.h"
#include "constants/pokemon.h"

EWRAM_DATA volatile struct UnkStruct_02024960 gUnknown_02024960 = {0};

u32 sub_02006490(void);
void sub_02002A9C(u8, u32, u8);
void sub_02002C44(void);
void sub_02006344(void);
void sub_02005BB8(void);
void FadeOut(void);
u32 sub_020047D4(void);
u32 sub_020064B0(void);
bool32 sub_02005704(u32);

bool8 sub_0200023C(void)
{
    u32 r0;

    sub_02006490();
    if (gUnknown_02024960.unk_03_0 == 0)
    {
        gUnknown_02024960.unk_858 = 1;
    }
    else
    {
        gUnknown_02024960.unk_858 = 2;
    }
    sub_02002A9C(0, sub_02006490(), 1);
    r0 = gUnknown_02024960.unk_87E | (gUnknown_02024960.unk_87B << 16);
    gUnknown_02024960.unk_880 = r0;
    gUnknown_02024960.unk_87F = 1;
    return FALSE;
}

extern IntrFunc *gUnknown_0201F4AC;
extern u8 gUnknown_020217B4;
extern u8 gUnknown_020217B8;

extern void sub_0200D924(const u8*);
extern void sub_0200D9EC(void);

struct RomHeader
{
    u8 a0[0xA8];
    u8 unkA8[8];
};

extern const struct RomHeader gRomHeader;

/*
static inline u32 Setunk20217B8(u32 a)
{
    u8 buffer[64];
    u32 var18 = 0;
    u32 r4 = 0;

    if (gAgbPmRomParams->gameLanguage != LANGUAGE_ENGLISH)
        return 0;

    while (1)
    {
        main_callback(&var18, buffer, a);
        if (var18 == 5)
            r4 = 1;
        if (var18 == 11)
            r4 = 2;
        if (var18 == 6 || var18 == 7)
            break;
        if (var18 == 11)
            break;
    }
    return r4;
}

void GF_Main(void)
{
    s32 i;
    u16 monStatus;

    DetectROM();
    sub_020098D8(2, gUnknown_0201F4AC);
    SaveBlocksInit();
    SetSaveSectorPtrs();
    gUnknown_020217B4 = ReadSaveBlockChunks();

    gUnknown_020217B8 = Setunk20217B8(0);

    InitSound();
    sub_02002C44();
    SetKeyRepeatTiming(0x28, 5);
    REG_IE = 1;
    REG_DISPSTAT = 8;
    REG_DISPCNT &= (0xFF7F);
    REG_IME = 1;
    sub_0200C9C0(gSaveBlock2Ptr, gSaveBlock1Ptr, gUnknown_020217B4);

    sub_0200D924(gRomHeader.unkA8);

    SetVBlankCallback(sub_0200D9EC);
    PauseSoundVSync();
    GenerateFontHalfrowLookupTable((u32 *) 0x03004000);
    FadeOut();
    for (;;)
    {
        if (sub_020047D4() == 0)
        {
            sub_02006344();
            sub_02005BB8();
            if (sub_020064B0() == 0)
            {
                sub_02005704((gUnknown_02024960.unk_03_0 != 0) ? gUnknown_02024960.unk_03_0 - 1 : 0);
            }
            else
            {
                sub_0200023C();
            }
        }
        else
        {
            CpuCopy16(gSaveBlock1BakPtr, gSaveBlock1Ptr, gAgbPmRomParams->saveBlock1Size);
            monStatus = 0;
            for (i = 0; i < PARTY_SIZE; i++)
            {
                SetMonData(&gPlayerPartyPtr[i], MON_DATA_STATUS, &monStatus);
            }
            sub_02002A9C(1, 0, 0);
        }
    }
}
*/
