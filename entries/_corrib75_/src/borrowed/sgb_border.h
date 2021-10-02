#ifndef __SGB_BORDER_H_INCLUDE
#define __SGB_BORDER_H_INCLUDE

#include <gb/gb.h>
#include <stdint.h>

#define SNES_RGB(R,G,B) (uint16_t)((B) << 10 | (G) << 5 | (R))

/** sets SGB border */

void set_sgb_border(unsigned char * tiledata, size_t tiledata_size,
                    unsigned char * tilemap, size_t tilemap_size,
                    unsigned char * palette, size_t palette_size) BANKED;

extern void sgb_transfer_nodelay(uint8_t * packet) __preserves_regs(b, c);
extern uint8_t sgb_check(void) __preserves_regs(b, c);

#endif
