;-------------------------------------------------------------------------------
    .area	_LCDVEC_HEADER (ABS)
    .org	0x48        ; LCD
.int_LCD:
    jp		_scanline_isr

;-------------------------------------------------------------------------------	
	.area	_CODE

_scanline_isr::
	push	af
	
	ld		a,(_LY_REG)
	cp		a,#0x8F
	jr		z,_end_screen
	cp		a,#0x3F
	jr		nc,_course1
	pop 	af
	reti

_course1::
	cp		a,#0x6F
	jr		c,_course2
	pop 	af
	reti
	
_course2::
	push	hl

	ld		l,a
	;ld		h,#0xC1
	ld		h,#0xD1
	ld		a,(hl)
	ld		(_SCX_REG),a

	ld		hl,#_player_count
	ld		a,(hl)
	ld		(_SCY_REG),a
	
	pop		hl
	pop 	af
	reti
	
_end_screen:
	push	hl
	
	ld		a,#0x00
	ld		(_SCY_REG),a		
	
	ld		hl,#_race_scroll
	ld		a,(hl)
	ld		(_SCX_REG),a
	
	pop		hl
	pop		af
	reti

