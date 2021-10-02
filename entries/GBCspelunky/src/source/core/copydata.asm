INCLUDE "hardware.inc"

SECTION "MEMFUNC", ROM0
; Copies data from one location to another using the CPU.
;
; Input:
; - `hl` = Destination
; - `bc` = Source
; - `de` = Byte count
;
; Output:
; - `hl` += Byte count
; - `bc` += Byte count
; - `de` = `$0000`
;
; Destroys: `a`
memcopy::

    ;Copy the data
    ld a, [bc]
    ld [hl+], a
    inc bc
    dec de

    ;Check byte count
    ld a, d
    or e
    jr nz, memcopy

    ;Return
    ret 
;



; Sets a number of bytes to a single value
;
; Input:
; - `hl` = Destination
; - `b` = Fill byte
; - `de` = Byte count
;
; Output:
; - `hl` += Byte count
; - `de` = `$0000`
;
; Destroys: `a`
memfill::

    ;Fill data
    ld a, b
    ld [hl+], a
    dec de

    ;Check byte count
    ld a, d
    or e
    jr nz, memfill

    ;Return
    ret
;



; Copies a palette to background color memory
;
; Input:
; - `hl` = Palette address
; - `a` = Palette index * 8
;
; Output:
; - `hl` += `$0010`
; - `a` += `$08`
;
; Destroys: `bc`
palette_copy_bg::

    ;Write palette index
    ld b, a
    set 7, a
    ldh [rBCPS], a
    ld c, low(rBCPD)

    ;Copy the palette
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a

    ;Increase palette index
    ld a, b
    add a, $08

    ;Return
    ret
;



; Copies a palette to sprite color memory
;
; Input:
; - `hl` = Palette address
; - `a` = Palette index * 8
;
; Output:
; - `hl` += `$0010`
; - `a` += `$08`
;
; Destroys: `bc`
palette_copy_spr::
    ;iHL = palette address
    ;iA  = pelette index * 8

    ;Write palette index
    ld b, a
    set 7, a
    ldh [rOCPS], a
    ld c, low(rOCPD)

    ;Copy the palette
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a
    ld a, [hl+]
    ldh [c], a

    ;Increase palette index
    ld a, b
    add a, $08

    ;Return
    ret
;