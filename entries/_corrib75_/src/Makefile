# Makefile for <corrib75>

# Alter these paths to match your environment.
# romusage, bgb, and SameBoy aren't needed to build the ROM.
# mod2gbt is only required if the Mod file has been changed.
#######

CC	= ~/Gameboy/gbdk/bin/lcc 
ROMUSAGE	= ~/Gameboy/romusage/bin/romusage
BGB 	= wine ~/Gameboy/bgb/bgb.exe 
#MOD2GBT = wine ~/Gameboy/gbt-player-2.1.1/mod2gbt/mod2gbt.exe
MOD2GBT = ~/Gameboy/gbt-player-2.1.1/mod2gbt/mod2gbt
SAMEBOY	= ~/Gameboy/SameBoy-0.14.3/sameboy

######

CFLAGS	= -Wf--Werror -Wb-yt0x1b -Wb-v -Wm-yt0x1b -Wm-ya1 -Wm-yo8 -Wm-yc -Wm-ys -Wm-yncorrib75

BIN	= corrib75.gb

all:	$(BIN)

sgb_border.o:	borrowed/sgb_border.c borrowed/sgb_border.h
	$(CC) $(CFLAGS) -c -o borrowed/sgb_border.o borrowed/sgb_border.c

gbt_player_bank1.o:	borrowed/gbt_player_bank1.s borrowed/gbt_player.h
	$(CC) $(CFLAGS) -S -o borrowed/gbt_player_bank1.o borrowed/gbt_player_bank1.s

gbt_player.o:	borrowed/gbt_player.s borrowed/gbt_player.h
	$(CC) $(CFLAGS) -S -o borrowed/gbt_player.o borrowed/gbt_player.s

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

assest/%.o:	assets/%.c
	$(CC) $(CFLAGS) -c -o assets/$@ assets/$<

%.o:	%.s
	$(CC) $(CFLAGS) -c -o $@ $<

%.s:	%.c
	$(CC) $(CFLAGS) -S -o $@ $<

assets/music1.c:	assets/Corrib75.mod
	$(MOD2GBT) assets/Corrib75.mod Music1 -c 6
	mv output.c assets/music1.c
	sed -i "s/bank=6/bank 6/" assets/music1.c

corrib75.gb:	code/main.o assets/bg_tiles.o code/palettes.o assets/sprite_tiles.o  code/levels.o code/player.o assets/easy.o code/misc.o borrowed/get_tile.o assets/alpha.o assets/music1.o borrowed/gbt_player_bank1.o borrowed/gbt_player.o assets/nowall.o code/lasers.o code/title.o code/messagebox.o assets/border_data.o code/player2.o borrowed/sgb_border.o borrowed/sgb2.o assets/pillars.o assets/rooms.o code/levels2.o code/npc.o assets/hextile.o assets/plane.o assets/logo_tiles.o assets/start_tiles.o assets/blocked.o code/pretitle.o assets/blobs.o assets/circles.o assets/blobs2.o assets/pretext.o assets/pretext_map.o code/intro.o assets/intro_map.o assets/blank.o assets/decay.o assets/reentry.o assets/easy2.o assets/boot.o assets/karen1.o assets/ai_map.o assets/triangles.o code/ending.o code/messageportrait.o code/postscript.o assets/ai_blank.o assets/meeting.o assets/profd.o code/message5.o assets/thanks.o assets/entry.o code/continue.o assets/continue_tiles.o code/glitch.o assets/chasm.o assets/frame_tiles.o assets/frame_map.o assets/beam2.o
	$(CC) $(CFLAGS) -o corrib75.gb borrowed/*.o assets/*.o code/*.o
	$(ROMUSAGE) $(BIN)

## Targets to rebuild and run:
urn:	run
run:	$(BIN)
	$(BGB) $(BIN) 

same:   $(BIN)
	$(SAMEBOY) $(BIN)

clean:
	rm -f *.o *.lst *.map *.gb *.ihx *.sym *.cdb *.adb *.asm *.noi
	rm -f assets/*.o assets/*.lst assets/*.map assets/*.gb assets/*.ihx assets/*.sym assets/*.cdb assets/*.adb assets/*.asm assets/*.noi
	rm -f borrowed/*.o borrowed/*.lst borrowed/*.map borrowed/*.gb borrowed/*.ihx borrowed/*.sym borrowed/*.cdb borrowed/*.adb borrowed/*.asm borrowed/*.noi
	rm -f code/*.o code/*.lst code/*.map code/*.gb code/*.ihx code/*.sym code/*.cdb code/*.adb code/*.asm code/*.noi

