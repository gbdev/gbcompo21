/*
    Copyright 2021 Gearóid Fox

    This file is part of <corrib75>.

    <corrib75> is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    <corrib75> is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with <corrib75>.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __misc_h_INCLUDE
#define __misc_h_INCLUDE

#define TRUE 1
#define FALSE 0

typedef uint8_t boolean;

extern void wait(uint8_t frames, uint8_t return_bank);
void load_logo7(void);
void end(void);

extern uint8_t btn;

extern uint8_t mask1;
extern uint8_t mask2;
extern uint8_t sprite_buffer[128];
extern uint8_t *obj_ptr1;
extern uint8_t *obj_ptr2;
extern uint8_t *obj_ptr3;
extern uint8_t *obj_ptr4;
extern uint8_t start_j;

extern uint16_t * const save_status;
extern uint8_t  * const save_data; 


#endif
