INCLUDE "hardware.inc"
INCLUDE "dwellings/tileid.inc"
INCLUDE "blockproperties.inc"
INCLUDE "banks.inc"
INCLUDE "entitysystem.inc"

SECTION "DWELLINGS MAP POLISH", ROMX, BANK[bank_dwellings_main]

; Polished the generated level, tile by tile.
;
; Input:
; `hl`: Pointer to block
dwellings_polish::
    
    ;Check block ID
    ld a, [hl]
    cp a, b_platform
    jp z, dwellings_polish_platform
    cp a, b_wood
    jp z, dwellings_polish_wood
    cp a, b_dirt
    jp z, dwellings_polish_dirt
    cp a, b_ground_chance
    jp z, dwellings_polish_ground_chance
    cp a, b_enemy_25
    jp z, dwellings_polish_enemy_25
    cp a, b_enemy_75
    jp z, dwellings_polish_enemy_75

    ;Nothin', just return
    ret 
;



;Create platform beams
dwellings_polish_platform:

    ;Maybe summon an enemy?
    call dwellings_polish_enemy_grounded
    
    ;Save HL on the stack
    push hl
    ld de, 64

    ;Loop
    .loop
        
        ;Add to offset
        add hl, de

        ;If background, convert and continue
        ld a, [hl]
        cp a, b_bg
        jr nz, :+
            ld [hl], b_platform_support
            jr .loop
        :

        ;If spikes, Convert and return
        cp a, b_spikes
        jr nz, :+
            ld [hl], b_spikes_support
            jr .postloop
        :

        ;Check block properties
        ld b, high(w_blockdata)
        ld c, a
        ld a, [bc]
        bit bpb_solid, a
        jr nz, .postloop
        bit bpb_ladder, a
        jr nz, .postloop
        bit bpb_platform, a
        jr z, .loop

    ;Pop HL and return
    .postloop
    pop hl
    ret 
;

;Create wooden backgrounds
;Also maybe summon an enemy
dwellings_polish_wood:

    ;Maybe summon an enemy
    call dwellings_polish_enemy_grounded
    call dwellings_polish_enemy_hanging
    
    ;Push HL for later
    push hl

    ;Check tile below, is it solid?
    ld de, 64
    add hl, de
    ld b, high(w_blockdata)
    ld a, [hl]

    ;Is this spikes?
    cp a, b_spikes
    jr nz, :+
        ld [hl], b_spikes_wood
        pop hl
        ret 
    :

    ;Is this a door?
    cp a, b_door_left
    jr nz, :+
        ld [hl], b_door_left_wood
        add hl, de
        ld [hl], b_door_left_wood
        pop hl
        ret 
    :

    ;Is this a door?
    cp a, b_door_right
    jr nz, :+
        ld [hl], b_door_right_wood
        add hl, de
        ld [hl], b_door_right_wood
        pop hl
        ret 
    :

    ;Is this NOT background?
    cp a, b_bg
    jr z, :+
        pop hl
        ret 
    :

    ;Move down
    add hl, de
    ld a, [hl]

    ;Is this a spike?
    cp a, b_spikes
    jr nz, :+
        ld [hl], b_spikes_wood
        pop hl
        push hl
        add hl, de
        ld [hl], b_wood_bg
        pop hl
        ret 
    :

    ;Is this solid?
    ld c, a
    ld a, [bc]
    and a, bpf_solid | bpf_platform
    jr z, :+
        pop hl
        push hl
        add hl, de
        ld [hl], b_wood_bg

        pop hl
        ret 
    :

    ld a, c
    cp a, b_bg
    jr nz, :+
        
        ;Check tile below, is it solid or a background?
        add hl, de
        ld c, [hl]
        ld a, [bc]
        and a, bpf_solid | bpf_platform
        jr z, :+

        pop hl
        push hl
        add hl, de
        ld [hl], b_wood_bg
        add hl, de
        ld [hl], b_wood_bg

        pop hl
        ret 
    :

    pop hl
    ret 






    ;Check tile below that
    add hl, de
    ld c, [hl]
    ld a, [bc]
    and a, bpf_solid | bpf_ladder | bpf_platform
    
    ;Check tile below, is it spikes?
    ld de, 64
    add hl, de
    ld a, [hl]
    cp a, b_spikes
    jr nz, :+

        ;Yes, convert and return
        ld [hl], b_spikes_wood
        pop hl
        ret 
    :

    ;Is it a background?
    cp a, b_bg
    jr z, :+

        ;No, pop and return
        pop hl
        ret 
    :

    ;Convert to wood BG
    ld [hl], b_wood_bg

    ;Check tile below, is it spikes?
    add hl, de
    ld a, [hl]
    cp a, b_spikes
    jr nz, :+

        ;Yes, convert and return
        ld [hl], b_spikes_wood
        pop hl
        ret 
    :

    ;Is it a background?
    cp a, b_bg
    jr z, :+

        ;No, pop and return
        pop hl
        ret 
    :

    ;Convert to wood BG
    ld [hl], b_wood_bg

    ;Pop and return
    pop hl
    ret 
;

;Might summon enemies
dwellings_polish_dirt:
    
    ;Maybe summon an enemy
    call dwellings_polish_enemy_grounded
    call dwellings_polish_enemy_hanging
    ret 
;

;Places a dirt block or an air block.
dwellings_polish_ground_chance:
    
    ;Get a random number
    call rng_run_single

    ;Place either dirt or air
    bit 0, a
    ld a, b_bg
    jr z, :+
    ld a, b_dirt
    :

    ;Write the chosen block
    ld [hl], a

    ;Was block air?
    cp a, b_bg
    ret nz

    ;Yes it was, check block below
    push hl
    ld a, l
    add a, 64
    ld l, a
    jr nc, :+
    inc h
    :

    ;Is it solid?
    ld d, high(w_blockdata)
    ld e, [hl]
    ld a, [de]
    bit bpb_solid, a
    jr z, .return

    ;Summon treasure
    ld bc, entity_treasure_init
    pop hl
    call dwellings_polish_enemy
    ret 

    ;Return peacefully
    .return
    pop hl
    ret 
;

;Maybe summon an enemy
dwellings_polish_enemy_25:

    ;Call RNG routine
    ld [hl], b_bg
    call rng_run_single
    and a, %00011000
    
    jr z, dwellings_polish_enemy
    ret 
;

;Maybe summon an enemy
dwellings_polish_enemy_75:
    
    ;Call RNG routine
    ld [hl], b_bg
    call rng_run_single
    and a, %00011000
    
    jr nz, dwellings_polish_enemy
    ret 
;

;Check if an enemy can be spawned (grounded)
dwellings_polish_enemy_grounded:
    
    ;Check if tile above is free
    push hl
    ld a, l
    sub a, 64
    ld l, a
    jr nc, :+
    dec h
    :
    ld c, [hl]
    ld b, high(w_blockdata)
    ld a, [bc]
    bit bpb_solid, a
    jr nz, .return

    ;Tile above is free
    call rng_run_single
    and a, %00011111
    jr nz, .return

    ;Figure out what kind of enemy to summon
    call rng_run_single
    and a, %00000011
    rla 
    ld d, high(dwellings_entities_ground)
    add a, low(dwellings_entities_ground)
    ld e, a
    ld a, [de]
    ld c, a
    inc de
    ld a, [de]
    ld b, a

    ;Summon that enemy
    call dwellings_polish_enemy
    ;Falls into label `.return`

    ;Return
    .return
    pop hl
    ret 
;

;Unused :(
dwellings_polish_enemy_hanging:
    ret 
;


; Summon an enemy.
; 
; Input:
; - `bc`: Entity initialization pointer
dwellings_polish_enemy:
    
    ;Save this for later
    push hl

    ;Allocate entity using initialization pointer in BC
    ld hl, entsys_alloc
    ld d, bank_entities
    call bank_call_xd

    ;Convert block pointer to XY-coordinates
    pop de
    push de
    ld a, d
    and a, %00001111
    ld d, a
    ld a, e
    rla 
    rl d
    rla 
    rl d
    ld a, %00111111
    and a, e
    ld e, a

    ;Set entity position
    ld a, l
    or a, entity_x
    ld l, a
    ld [hl], e
    inc l
    inc l
    ld [hl], d

    ;Restore WRAMX bank, clean up and return
    ld a, bank_foreground
    ldh [rSVBK], a
    pop hl
    ret 
;