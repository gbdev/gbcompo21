static UINT8 MusicTimer = 0;
static UINT8 AnimationTicker = 0;


unsigned char EarthMusic[] =
{
  710 , 0x00 , 710 , 783 , 710 , 0x00 , 0x00 , 0x00 , 0x00 , 710 , 0x00 , 786 , 710 , 0x00 , 786 , 923 , 786 , 710 , 0 , 786 , 710, 0x00 , 0x00 , 0x00 
};


void PlayMusic(unsigned char Music[] , UINT8 Size) {

	if (Music[MusicTimer] == 0){
		NR52_REG = 0x00;
	}
	else {
		NR52_REG = 0x80;
	}
	NR51_REG = 0x11;
	NR50_REG = 0x77;

	NR10_REG = 0x00;
	NR11_REG = 0x81;
	NR12_REG = 0x43;
	NR13_REG = Music[MusicTimer];
	NR14_REG = 0x85;

	if (MusicTimer >= Size){
		MusicTimer = 0;
	}
	else {
		if (AnimationTicker == 10)MusicTimer ++ ;
		if (AnimationTicker == 20)MusicTimer ++ ;
		if (AnimationTicker == 30)MusicTimer ++ ;
		if (AnimationTicker == 40)MusicTimer ++ ;
		if (AnimationTicker == 50)MusicTimer ++ ;
		if (AnimationTicker == 60)MusicTimer ++ ;
		if (AnimationTicker == 70)MusicTimer ++ ;
		if (AnimationTicker == 80)MusicTimer ++ ;
		if (AnimationTicker == 90)MusicTimer ++ ;
		if (AnimationTicker == 100)MusicTimer ++ ;
	}

}
