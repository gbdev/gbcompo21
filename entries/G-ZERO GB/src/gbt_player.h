/*
 * GBT Player v2.1.3
 *
 * SPDX-License-Identifier: MIT
 *
 * Copyright (c) 2009-2020, Antonio Niño Díaz <antonio_nd@outlook.com>
 */

#ifndef _GBT_PLAYER_
#define _GBT_PLAYER_

#include <gb/gb.h>

void gbt_play(void *data, UINT8 bank, UINT8 speed);
void gbt_stop(void);
void gbt_mute_channel1(UINT8 mute);
void gbt_mute_channel2(UINT8 mute);
void gbt_mute_channel4(UINT8 mute);
void gbt_update(void);

#define GBT_CHAN_1 (1<<0)
#define GBT_CHAN_2 (1<<1)
#define GBT_CHAN_3 (1<<2)
#define GBT_CHAN_4 (1<<3)

#endif
