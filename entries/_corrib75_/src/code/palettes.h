#ifndef __palettes_h_INCLUDE
#define __palettes_h_INCLUDE

#define PALETTE_BANK 2

void dim_buffer(uint8_t n);
void set_pal_from_buffer(void);
void load_pal_buffer(void);

extern const uint16_t black_pal[];
extern const uint16_t box1_pal[];
extern const uint16_t box2_pal[];
extern const uint16_t exclam_pal[];
extern const uint16_t tele_pal_rgb9[];
extern const uint16_t player_pal_rb0[];
extern const uint16_t player_pal_rgb1[];
extern const uint16_t floor_pal_rgb0[];
extern const uint16_t floor_pal_rgb1[];
extern const uint16_t floor_pal_rgb2[];
extern const uint16_t profd_pal_rgb0[];
extern const uint16_t ai_pal_rgb[];
extern const uint16_t wall_pal_rgb0[];
extern const uint16_t wall_pal_rgb2[];
extern const uint16_t wall_pal_rgb1[];
extern const uint16_t npc_pal_rgb0[];
extern const uint16_t npc_pal_rgb1[];
extern const uint16_t npc_pal_rgb2[];
extern uint8_t bg_pal_buffer[96];
extern uint8_t fg_pal_buffer[96];
#endif
