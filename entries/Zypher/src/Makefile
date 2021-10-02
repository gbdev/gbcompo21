CC	= C:/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG

all: main.c
		$(CC) -c -o Main.o Main.c
		$(CC) -Wf-ba0 -c -o BankOne.o LevelBanks/BankOne.c
		$(CC) -Wf-bo1 -Wf-ba1 -c -o BankTwo.o LevelBanks/BankTwo.c
		$(CC) Main.o BankOne.o BankTwo.o -Wl-yt2 -Wl-yo4 -Wl-ya4 -DUSE_SFR_FOR_REG -o Zypher.gb
clean:
	ECHO "Removing All Files"
	rm *.o
	rm *.lst
	rm *.asm
	rm *.sym
	rm *.ihx
	rm *.map
	rm *.noi
	ECHO "Done"