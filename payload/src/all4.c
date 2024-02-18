#include "global.h"
#include "gflib.h"
#include "all.h"
#include "strings.h"
#include "graphics.h"
#include "unk_200C5DC.h"
#include "gflib/text.h"
#include "gflib/gfxload.h"
#include "libpmagb/pic_uncomp.h"
#include "libpmagb/agb_rom.h"
#include "pokemon.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/songs.h"

extern void sub_02002C80(void);

extern const struct Window gUnknown_0201FE3C;
extern const struct Window gUnknown_0201FE5C;

extern const struct Subsprites *gUnknown_0201FC44[];
extern const struct Subsprites gUnknown_0201FC4C[];

struct Unk2021A20Str
{
    struct Sprite *unk0;
    struct Sprite *unk4;
    struct Sprite *unk8;
    u16 unkC;
    u16 unkE;
    u8 unk10;
    u8 fill11[3];
    struct Window *unk14;
    struct Window *unk18;
};

extern struct Unk2021A20Str gUnknown_02021A20;

void sub_02004AC4(void)
{
    ClearVram();
    REG_DISPCNT = DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP;
    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(27);
    REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(15);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(28) | BGCNT_TXT512x256;
    REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(30);
    AutoUnCompVram(gUnknown_0201B1A0, (void *) PLTT);
    AutoUnCompVram(gMonInfoIcons_Gfx, (void *) VRAM);
    AutoUnCompVram(gBattleScreen_Gfx, (void *) VRAM + 0x8000);
    AutoUnCompVram(gBattleScreen_Pal, (void *) PLTT + 0x140);

    AutoUnCompVram(gBattleScreen_Tilemap, (void *) IWRAM_START + 0x3000);
    AutoUnCompVram(gBattleWindows_Tilemap, (void *) IWRAM_START + 0x2800);

    gBgTilemapBufferTransferScheduled[2] = TRUE;
    gBgTilemapBufferTransferScheduled[3] = TRUE;

    AutoUnCompVram(gTypeStatusIcons_Pal, (void *) PLTT + 0x200);
    AutoUnCompVram(gTypeStatusIcons_Gfx, (void *) VRAM + 0x10000);

    *(u16 *)(PLTT) = 0x5108;

    CpuFill16(0, &gUnknown_02021A20, sizeof(gUnknown_02021A20));

    gUnknown_02021A20.unk14 = AddWindow(0, &gUnknown_0201FE3C);
    gUnknown_02021A20.unk18 = AddWindow(1, &gUnknown_0201FE5C);

    SetTextColor(gUnknown_02021A20.unk14, 1, 8);
    SetTextColor(gUnknown_02021A20.unk18, 1, 8);
}

void sub_02004BEC(struct Unk2021A20Str *a0, u32 monId, s32 x, s32 y)
{
    void *buffer;
    u32 personality;
    const struct CompressedSpriteSheet *monFrontSheet;
    const struct SpeciesInfo *speciesInfo = gAgbPmRomParams->baseStats;

    a0->unkC = gUnknown_02024960.unk10;
    if (a0->unkC == SPECIES_NONE)
        return;

    personality = gUnknown_02024960.unk8;
    a0->unkE = FixUnownSpecies(a0->unkC, personality);
    if (a0->unk0 != NULL)
        MoveSpriteToHead(a0->unk0);

    a0->unk0 = AddSprite(x, y, gUnknown_0201FC44[speciesInfo[a0->unkC].noFlip]);
    SetSpritePaletteNum(a0->unk0, 4);
    monFrontSheet = &gAgbPmRomParams->monFrontPicTable[a0->unkE];
    buffer = GetPicUncompPtr();
    LZ77UnCompVram(monFrontSheet->data, buffer);
    if (a0->unkC == SPECIES_CASTFORM)
        buffer += (gUnknown_02024960.unk12 * 0x800);
    DrawSpindasSpots(a0->unkC, personality, buffer);
    CpuCopy16(buffer, (void *)VRAM + 0x12000, 0x800);
    LZ77UnCompWram(GetMonPalettePtrBySpeciesIdPersonality(a0->unkC, gUnknown_02024960.unkC, personality)->data, buffer);
    if (a0->unkC == SPECIES_CASTFORM)
        buffer += (gUnknown_02024960.unk12 * 32);
    CpuCopy16(buffer, (void *)PLTT + 0x280, 0x20);
}

void sub_02004CE4(struct Pokemon *mon)
{
    u8 text[16];
    u32 gender = GetMonGender(mon);
    if (gender == MON_MALE || gender == MON_FEMALE)
    {
        text[1] = EOS;
        TextWindowSetXY(gUnknown_02021A20.unk14, 72, 16);
        if (gender == MON_MALE)
        {
            text[0] = CHAR_MALE;
            SetTextColor(gUnknown_02021A20.unk14, 4, 5);
        }
        else
        {
            text[0] = CHAR_FEMALE;
            SetTextColor(gUnknown_02021A20.unk14, 7, 8);
        }
        RenderText(gUnknown_02021A20.unk14, text);
        SetBgTilemapBufferPaletteRect(0, 20, 8, 1, 2, 0);
        SetTextColor(gUnknown_02021A20.unk14, 1, 8);
    }
}

void sub_02004D68(u32 monId)
{
    s32 i;
    u8 text[32];
    u32 level, species;
    s32 var;
    struct Pokemon *mon = &gPlayerPartyPtr[monId];

    sub_02004BEC(&gUnknown_02021A20, monId, 8, 32);
    text[0] = 0x34;
    text[1] = EOS;
    TextWindowSetXY(gUnknown_02021A20.unk14, 8, 0);
    RenderText(gUnknown_02021A20.unk14, text);

    level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    NumToPmString3CustomZeroChar(level, text, CHAR_0);
    for (i = 0; text[i] == CHAR_0; i++)
        ;
    if (text[i] == EOS)
        i--;
    TextWindowSetXY(gUnknown_02021A20.unk14, 17, 0);
    RenderText(gUnknown_02021A20.unk14, text + i);

    GetMonData(mon, MON_DATA_NICKNAME, text);
    BufferString(4, text);
    TextWindowSetXY(gUnknown_02021A20.unk14, 44, 0);
    RenderText(gUnknown_02021A20.unk14, text);

    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    if (species != SPECIES_NIDORAN_M && species != SPECIES_NIDORAN_F)
        sub_02004CE4(mon);

    GetSpeciesName(text, species);
    TextWindowSetXY(gUnknown_02021A20.unk14, 8, 16);
    RenderText(gUnknown_02021A20.unk14, text);

    var = GetMonStatus(mon);
    if (var != STATUS_PRIMARY_NONE)
    {
        if (gUnknown_02021A20.unk4 == NULL)
            gUnknown_02021A20.unk4 = AddSprite(166, 68, gUnknown_0201FC4C);
        SetSpriteInvisible(gUnknown_02021A20.unk4, FALSE);
        SetSpriteTileOffset(gUnknown_02021A20.unk4, (var - 1) * 4);
    }

    DrawPartyMonHealthBar(1, 12, 10, monId);
    SetBgPos(1, 0, -4);
    gBgTilemapBufferTransferScheduled[1] = TRUE;

    var = GetMonData(mon, MON_DATA_HP, NULL);
    NumToPmString3RightAlign(text, var);
    text[3] = CHAR_SLASH;
    var = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    NumToPmString3RightAlign(text + 4, var);
    TextWindowSetXY(gUnknown_02021A20.unk14, 80, 32);
    gUnknown_02021A20.unk14->glyphWidth = 6;
    RenderText(gUnknown_02021A20.unk14, text);
    gBgTilemapBufferTransferScheduled[0] = TRUE;
}

struct StrCoords
{
    u8 x;
    u8 y;
    u8 unk2;
    const u8 *str;
    void (*callback)(s32 id);
};

extern const struct Subsprites gUnknown_0201FC64[];
extern const struct Subsprites gUnknown_0201FCA0[];
extern const struct Subsprites gUnknown_0201FCF4[];
extern const struct Subsprites gUnknown_0201FD60[];

extern const struct StrCoords gText_BattleOptions[];
extern const struct StrCoords gUnknown_0201FDC4[];
extern const struct StrCoords gUnknown_0201FDF4[];
extern const struct StrCoords gUnknown_0201FE24[];

static inline void sub_02004F04Inline(struct Window *window, const struct StrCoords *strInfo, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++, strInfo++)
        RenderTextAt(window, strInfo->x - (window->left * 8), strInfo->y - (window->top * 8), strInfo->str);
}

void sub_02004F04(struct Window *window, const struct StrCoords *strInfo, s32 count)
{
    sub_02004F04Inline(window, strInfo, count);

    if (gUnknown_02021A20.unk8 != NULL)
    {
        MoveSpriteToHead(gUnknown_02021A20.unk8);
        gUnknown_02021A20.unk8 = NULL;
    }

    if (strInfo == gText_BattleOptions)
        gUnknown_02021A20.unk8 = AddSprite(0, 0, gUnknown_0201FC64);
    else if (strInfo == gUnknown_0201FDC4)
        gUnknown_02021A20.unk8 = AddSprite(0, 0, gUnknown_0201FCA0);
    else if (strInfo == gUnknown_0201FDF4)
        gUnknown_02021A20.unk8 = AddSprite(0, 0, gUnknown_0201FCF4);
    else if (strInfo == gUnknown_0201FE24)
        gUnknown_02021A20.unk8 = AddSprite(0, 0, gUnknown_0201FD60);

    SetSpritePaletteNum(gUnknown_02021A20.unk8, 5);
    SetSpritePos(gUnknown_02021A20.unk8, strInfo[0].x, strInfo[0].y);
}

s32 sub_02004FB8(const struct StrCoords *strInfo, s32 count)
{
    u32 keys;
    const struct StrCoords *curr;
    s32 i, previousId;

    for (i = 0; strInfo[i].str[0] == EOS; i++)
        ;

    if (strInfo == gUnknown_0201FDF4 && i == 1 && strInfo[2].str[0] != EOS)
        i = 2;

    if (i < count)
    {
        SetSpritePos(gUnknown_02021A20.unk8, strInfo[i].x, strInfo[i].y);
        curr = &strInfo[i];
        while (1)
        {
            DelayFrames(1);
            keys = gNewKeys;
            previousId = i;
            if (keys & DPAD_RIGHT)
            {
                if (i == 0)
                {
                    if (strInfo[1].str[0] != EOS && strInfo[1].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[1];
                        i = 1;
                    }
                    else if (strInfo[3].str[0] != EOS && strInfo[3].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[3];
                        i = 3;
                    }
                }
                if (i == 2)
                {
                    if (strInfo[3].str[0] != EOS && strInfo[3].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[3];
                        i = 3;
                    }
                    else if (strInfo[1].str[0] != EOS && strInfo[1].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[1];
                        i = 1;
                    }
                }
            }
            if (keys & DPAD_LEFT)
            {
                if (i == 1)
                {
                    if (strInfo[0].str[0] != EOS && strInfo[0].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[0];
                        i = 0;
                    }
                    else if (strInfo[2].str[0] != EOS && strInfo[2].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[2];
                        i = 2;
                    }
                }
                if (i == 3)
                {
                    if (strInfo[2].str[0] != EOS && strInfo[2].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[2];
                        i = 2;
                    }
                    else if (strInfo[0].str[0] != EOS && strInfo[0].str[0] != CHAR_HYPHEN)
                    {
                        curr = &strInfo[0];
                        i = 0;
                    }
                }
            }
            if (keys & DPAD_DOWN)
            {
                if (i + 2 < count && curr[2].str[0] != EOS)
                {
                    curr += 2;
                    i += 2;
                }
            }
            if (keys & DPAD_UP)
            {
                if (i - 2 >= 0 && curr[-2].str[0] != EOS)
                {
                    curr -= 2;
                    i -= 2;
                }
            }

            if (previousId != i)
            {
                PlaySE(SONG_SE_SELECT);
                SetSpritePos(gUnknown_02021A20.unk8, curr->x, curr->y);
                if (curr->callback != NULL)
                    curr->callback(i);
            }

            if (keys & B_BUTTON)
            {
                PlaySE(SONG_SE_SELECT);
                return 0;
            }
            if (keys & A_BUTTON)
            {
                PlaySE(SONG_SE_SELECT);
                gUnknown_02021A20.unk10 = i;
                return curr->unk2;
            }
        }
    }
    return 0;
}

static u32 gUnknown_02020A48;

void sub_02005168(s32 id)
{
    u8 text[16];
    s32 ppBonuses, ppTotal, ppCurr, type;
    struct Pokemon *mon = &gPlayerPartyPtr[gUnknown_02020A48];
    const struct BattleMove *battleMoves = gAgbPmRomParams->battleMoves;
    u32 move = gUnknown_02024960.unk14[id];

    FillWindowCharBufferRect(gUnknown_02021A20.unk18, 22, 2, 6, 2, 0);
    type = battleMoves[move].type;
    RenderTextAt(gUnknown_02021A20.unk18, 176, 16, gTypeNames[type]);

    FillWindowCharBufferRect(gUnknown_02021A20.unk18, 24, 0, 4, 2, 0);
    ppCurr = gUnknown_02024960.unk1C[id];
    ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES, NULL);
    ppTotal = CalculatePPWithBonus(move, ppBonuses, id);
    NumToPmString3RightAlign(text, ppCurr);
    text[3] = CHAR_SLASH;
    NumToPmString3RightAlign(text + 4, ppTotal);
    text[4] = text[5];
    text[5] = text[6];
    text[6] = text[7];
    RenderTextAt(gUnknown_02021A20.unk18, 191, 0, text);
}

s32 sub_02005264(s32 monId)
{
    u8 text[16];
    s32 i, count;

    CopyRectWithinBgTilemapBuffer(2, 0, 38, 30, 6, 0, 14);
    FillBgTilemapBufferRect(3, 0, 14, 30, 6, 0);
    ClearWindowCharBuffer(gUnknown_02021A20.unk18, 0);
    RenderTextAt(gUnknown_02021A20.unk18, 176, 0, gText_PP);
    sub_02005168(0);

    for (count = 0, i = 0; i < MAX_MON_MOVES; i++)
    {
        u32 move = gUnknown_02024960.unk14[i];
        if (move != MOVE_NONE)
        {
            CopyMoveName(text, move);
            count++;
        }
        else
        {
            StringCopy(text, gText_1Dash);
        }
        BufferString(i, text);
    }

    sub_02004F04(gUnknown_02021A20.unk18, gUnknown_0201FDC4, 4);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    gBgTilemapBufferTransferScheduled[3] = TRUE;

    return count;
}

extern u8 gUnknown_02021A00[];
extern u8 gStringBuffers[][32];

u8 *sub_0200531C(s32 id)
{
    u8 UNUSED unusedVar[4];
    u8 text[16];
    s32 len, i;
    u8 *txtPtr = gUnknown_02021A00;

    CpuFill16(0, txtPtr, 0x20);
    if (gUnknown_02024960.unk81C[id].str[0] == EOS)
    {
        txtPtr[0] = EOS;
    }
    else
    {
        txtPtr[0] = gUnknown_02024960.unk81C[id].unk826_0 + CHAR_0;
        txtPtr[1] = CHAR_P;
        txtPtr[2] = CHAR_COLON;
        txtPtr[3] = CHAR_SPACE;
        len = 4;
        if (gUnknown_02024960.unk81C[id].unk826_1)
        {
            txtPtr[4] = EXT_CTRL_CODE_BEGIN;
            txtPtr[5] = 0x15;
            len = 6;
        }
        for (i = 0; i < gAgbPmRomParams->pokemonNameLength_2; i++)
        {
            if (gUnknown_02024960.unk81C[id].str[i] == EOS)
                break;
            txtPtr[i + len] = gUnknown_02024960.unk81C[id].str[i];
        }
        i += len;
        if (gUnknown_02024960.unk81C[id].unk826_3 || gUnknown_02024960.unk81C[id].unk826_2)
        {
            u32 species;

            if (gUnknown_02024960.unk81C[id].unk826_3)
                species = SPECIES_NIDORAN_M;
            else
                species = SPECIES_NIDORAN_F;
            GetSpeciesName(text, species);
            if (StringCompare((u8 *)gUnknown_02024960.unk81C[id].str, text) == 0)
            {
                if (gUnknown_02024960.unk81C[id].unk826_4 == 0)
                    txtPtr[i++] = CHAR_MALE;
                else if (gUnknown_02024960.unk81C[id].unk826_4 == 1)
                    txtPtr[i++] = CHAR_FEMALE;
            }
        }
        else
        {
            if (gUnknown_02024960.unk81C[id].unk826_4 == 0)
                txtPtr[i++] = CHAR_MALE;
            else if (gUnknown_02024960.unk81C[id].unk826_4 == 1)
                txtPtr[i++] = CHAR_FEMALE;
        }
        txtPtr[i] = EOS;
    }

    return txtPtr;
}

s32 sub_02005468(void)
{
    u8 text[1];

    text[0] = EOS;
    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    FillBgTilemapBufferRect(2, 0, 14, 30, 6, 0);
    ClearWindowCharBuffer(gUnknown_02021A20.unk18, 0xFFFF);
    StringCopy(gStringBuffers[0], sub_0200531C(0));
    StringCopy(gStringBuffers[1], sub_0200531C(1));
    if (gUnknown_02024960.unk7 == 4)
    {
        StringCopy(gStringBuffers[2], sub_0200531C(2));
        StringCopy(gStringBuffers[3], sub_0200531C(3));
    }
    else
    {
        StringCopy(gStringBuffers[2], text);
        StringCopy(gStringBuffers[3], text);
    }
    sub_02004F04(gUnknown_02021A20.unk18, gUnknown_0201FDF4, 4);
    gBgTilemapBufferTransferScheduled[3] = TRUE;
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    return gUnknown_02024960.unk7;
}

s32 sub_02005548(void)
{
    s32 var;

    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    FillBgTilemapBufferRect(2, 0, 14, 30, 6, 0);
    ClearWindowCharBuffer(gUnknown_02021A20.unk18, 0xFFFF);
    RenderTextAt(gUnknown_02021A20.unk18, 0, 0, gText_WillYouGiveIn);
    sub_02004F04(gUnknown_02021A20.unk18, gUnknown_0201FE24, 2);
    gBgTilemapBufferTransferScheduled[3] = TRUE;
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    var = sub_02004FB8(gUnknown_0201FE24, 2);
    if (var != 1)
        ClearWindowCharBuffer(gUnknown_02021A20.unk18, 0);
    return var;
}

static inline void sub_2005E68(void)
{
    CopyRectWithinBgTilemapBuffer(2, 0, 32, 30, 6, 0, 14);
    FillWindowCharBufferRect(gUnknown_02021A20.unk18, 18, 0, 10, 4, 0);
    RenderTextAt(gUnknown_02021A20.unk18, 0, 0, gText_WhatWillMonDo);
    sub_02004F04(gUnknown_02021A20.unk18, gText_BattleOptions, 4);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
}

static inline void sub_2005F08(u32 unused, s32 moveSlot)
{
    u8 text[28];
    u32 move = gUnknown_02024960.unk14[moveSlot];
    if (move != MOVE_NONE)
        CopyMoveName(text, move);
    else
        StringCopy(text, gText_1Dash);
    BufferString(0, text);
}

void sub_020055D4(s32 monId, s32 moveSlot, u32 stringId, bool32 arg3)
{
    u8 text[24];

    FillBgTilemapBufferRect(2, 0, 14, 30, 6, 0);
    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    ClearWindowCharBuffer(gUnknown_02021A20.unk18, 0);
    MoveSpriteToHead(gUnknown_02021A20.unk8);
    gUnknown_02021A20.unk8 = NULL;

    if (moveSlot != 0)
        sub_2005F08(0, moveSlot - 1);

    GetMonData(&gPlayerPartyPtr[monId], MON_DATA_NICKNAME, text);
    BufferString(4, text);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    gBgTilemapBufferTransferScheduled[3] = TRUE;
    RenderText(gUnknown_02021A20.unk18, gBattleStringsTable[stringId]);
    if (!arg3)
    {
        ClearWindowCharBuffer(gUnknown_02021A20.unk18, 0);
        sub_2005E68();
    }
}

u32 sub_02006414(s32 moveSlot);
u32 sub_0200645C(void);
u32 sub_0200644C(void);
u32 sub_02006430(s32 moveSlot);

static inline bool32 sub_2005F44(s32 monId, s32 moveSlot)
{
    switch (sub_02006414(moveSlot - 1))
    {
    case 0:
        return TRUE;
    case 1:
        sub_020055D4(monId, moveSlot, 4, FALSE);
        break;
    case 2:
        sub_020055D4(monId, moveSlot, 5, FALSE);
        break;
    case 3:
        sub_020055D4(monId, moveSlot, 6, FALSE);
        break;
    case 4:
        sub_020055D4(monId, moveSlot, 7, FALSE);
        break;
    case 5:
        sub_020055D4(monId, sub_0200645C(), 8, FALSE);
        break;
    case 6:
        sub_020055D4(monId, moveSlot, 9, FALSE);
        break;
    }
    return FALSE;
}

static inline void sub_2005ECC(void)
{
    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    ClearWindowCharBuffer(gUnknown_02021A20.unk18, 0);
    gBgTilemapBufferTransferScheduled[3] = TRUE;
}

struct TstStruct
{
    u8 state;
    u8 u1;
    u8 moveSlot;
};

extern struct TstStruct gUnknown_020219F0;

bool32 sub_02005704(u32 monId)
{
    u32 r2;
    bool32 loop;
    u32 r9;
    struct TstStruct *r7 = &gUnknown_020219F0;

    gUnknown_02020A48 = monId;
    sub_02004AC4();
    sub_02004D68(monId);
    sub_2005E68();
    FadeIn();
    loop = TRUE;
    while (loop)
    {
        switch (r7->state)
        {
        case 0:
            sub_2005E68();
            r7->state = sub_02004FB8(gText_BattleOptions, 4);
            if (r7->state == 0 && gUnknown_02024960.unk_03_0 == 2)
            {
                gUnknown_02024960.unk_87B = 0;
                loop = FALSE;
            }
            break;
        case 1:
            if (sub_0200644C() == 1)
            {
                sub_020055D4(gUnknown_02020A48, 0, 14, 1);
                gUnknown_02024960.unk_87C = 0;
                gUnknown_02024960.unk_87B = 1;
                r7->state = 0;
                loop = FALSE;
            }
            else if (sub_0200644C() == 2)
            {
                gUnknown_02024960.unk_87C = 0;
                gUnknown_02024960.unk_87B = 1;
                r7->state = 0;
                loop = FALSE;
            }
            else
            {
                u32 var = sub_02005264(monId);
                r7->moveSlot = sub_02004FB8(gUnknown_0201FDC4, var);
                if (r7->moveSlot == 0)
                {
                    r7->state = 0;
                    sub_2005ECC();
                }
                else if (sub_02006430(r7->moveSlot - 1) == 0)
                {
                    if (sub_2005F44(gUnknown_02020A48, r7->moveSlot))
                    {
                        gUnknown_02024960.unk_87C = r9,
                        gUnknown_02024960.unk_87B = 1,
                        loop = FALSE;
                    }
                    r7->state = 0;
                }
                else
                {
                    r7->state = 4;
                }
            }
            break;
        case 2:
            r9 = sub_02005548();
            r7->state = 0;
            if (r9 == 1)
            {
                gUnknown_02024960.unk_87B = 3;
                loop = FALSE;
            }
            break;
        case 3:
            FadeOut();
            sub_02002C80();
            if (gUnknown_02024960.unk_03_0 == 0)
                gUnknown_02024960.unk_858 = 1;
            else
                gUnknown_02024960.unk_858 = 2;
            sub_02002A9C(0, monId, 0);
            r7->state = 0;
            if (gUnknown_02024960.unk_87B != 4)
            {
                loop = FALSE;
            }
            else
            {
                sub_02004AC4();
                sub_02004D68(monId);
                sub_2005E68();
                FadeIn();
            }
            break;
        case 4:
            sub_02005468();
            r9 = sub_02004FB8(gUnknown_0201FDF4, 4);
            if (r9 == 0)
            {
                r7->state = 1;
            }
            else
            {
                if (sub_2005F44(gUnknown_02020A48, r7->moveSlot))
                {
                    gUnknown_02024960.unk_87C = r9,
                    gUnknown_02024960.unk_87B = 1,
                    loop = FALSE;
                }
                r7->state = 0;
            }
            break;
        }

        DelayFrames(1);
    }

    r2 = 0;
    switch (gUnknown_02024960.unk_87B)
    {
    case 1:
        r2 = r7->moveSlot - 1;
        r2 |= (((gUnknown_02024960.unk_87C - 1) << 8) & 0xFF00);
        break;
    case 2:
        r2 = gUnknown_02024960.unk_87E;
        break;
    }

    r2 |= (gUnknown_02024960.unk_87B) << 16;
    gUnknown_02024960.unk_880 = r2;
    gUnknown_02024960.unk_87F = 1;

    if (!IsScreenFadedOut())
        FadeOut();

    return FALSE;
}
