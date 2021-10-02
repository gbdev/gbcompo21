set BIN=..\..\bin
set OBJ=obj

if not exist %OBJ% mkdir %OBJ%

%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\main.o main.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\global.o global.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\data.o data.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\player.o player.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\race.o race.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\race_finished.o race_finished.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\end.o end.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\title.o title.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\music.o music.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\sound.o sound.c
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\int.o int.s
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\gbt_player.o gbt_player.s
%BIN%\lcc -Wa-l -Wl-m -Wl-j -c -o %OBJ%\unapack.o unapack.s
%BIN%\lcc -Wa-l -Wl-m -Wl-j -o gzero.gb %OBJ%\main.o %OBJ%\global.o %OBJ%\data.o %OBJ%\player.o^
 %OBJ%\race.o %OBJ%\race_finished.o %OBJ%\end.o %OBJ%\title.o %OBJ%\music.o %OBJ%\sound.o %OBJ%\int.o %OBJ%\gbt_player.o^
 %OBJ%\unapack.o

del *.ihx
del *.map
del *.noi

pause
