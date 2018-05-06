#include "project.h"
#include "pic32mx.h"
#define SKULL_SIZE 12

const char crown[PLAYER_SIZE][PLAYER_SIZE]={
    {1,1,1,1,1,1,1,1},
	{0,0,0,1,1,1,1,1},
	{0,0,0,0,0,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{0,0,0,1,1,1,1,1},
	{0,0,0,0,0,1,1,1},
	{1,1,1,1,1,1,1,1}
};
char pixel_line[128][2];
//skull is 12x12
const char skull[SKULL_SIZE][SKULL_SIZE]={
    {0,0,0,1,1,1,1,1,0,0,0,0},
	{0,0,0,1,1,1,1,1,0,0,0,0},
	{1,1,1,1,1,0,0,1,1,1,1,1},
	{1,1,1,1,1,0,0,1,1,1,0,0},
	{1,1,1,1,1,0,0,1,1,1,0,0},
	{1,1,1,1,1,1,1,1,0,0,1,1},
	{1,1,1,1,1,1,1,1,0,0,1,1},
	{1,1,1,1,1,0,0,1,1,1,0,0},
	{1,1,1,1,1,0,0,1,1,1,0,0},
	{1,1,1,1,1,0,0,1,1,1,1,1},
	{0,0,0,1,1,1,1,1,0,0,0,0},
	{0,0,0,1,1,1,1,1,0,0,0,0}
};
const char smiley[SKULL_SIZE][SKULL_SIZE]={
    {0,0,0,1,1,1,1,1,1,0,0,0},
	{0,0,1,1,1,1,1,1,1,1,0,0},
	{0,1,1,0,0,0,1,1,1,1,1,0},
	{1,1,1,0,0,0,1,0,0,1,1,1},
	{1,1,1,0,0,0,1,1,0,0,1,1},
	{1,1,1,1,1,1,1,1,1,0,1,1},
	{1,1,1,1,1,1,1,1,1,0,1,1},
	{1,1,1,0,0,0,1,1,1,0,1,1},
	{1,1,1,0,0,0,1,1,0,0,1,1},
	{0,1,1,0,0,0,1,0,0,1,1,0},
	{0,0,1,1,1,1,1,1,1,1,0,0},
	{0,0,0,1,1,1,1,1,1,0,0,0}
};

void wait_for_button_release(){
	while(1){
	int btn_data = getbtns();
	if(!(btn_data&12))
	break;
	}

}
int menu_init(void){
	int x;
	int players = 1;
	for(x=0;x<128;x++){
		pixel_line[x][0] = 1;
		pixel_line[x][1] = 1;
	}
	//Show player selection
	while(1){
	int btn_data = getbtns();

	display_zero();
	if(players == 1){
		display_object(28,10,PLAYER_SIZE,PLAYER_SIZE,(const char *)crown);
	}
	else{
		display_object(86,10,PLAYER_SIZE,PLAYER_SIZE,(const char *)crown);
		display_object(106,10,PLAYER_SIZE,PLAYER_SIZE,(const char *)crown);
	}
	display_object(28,20,PLAYER_SIZE,PLAYER_SIZE,(const char *)player_object);
	display_object(86,20,PLAYER_SIZE,PLAYER_SIZE,(const char *)player_object);
	display_object(106,20,PLAYER_SIZE,PLAYER_SIZE,(const char *)player_object);
	display_object(64,0,2,128,(const char *)pixel_line);
	display_update();
	if(btn_data&4){
		wait_for_button_release();
		if(players==1)
		players = 2;
		else
		players = 1;
	}
	if(btn_data&8){
		wait_for_button_release();
		break;
	}

	}
	int difficulty = 1;
	//Show difficulty Selection
	while(1){
		int btn_data = getbtns();
		display_zero();
		display_object(64,0,2,128,(const char *)pixel_line);
		display_object(26,16,SKULL_SIZE,SKULL_SIZE,(const char *)smiley);
		display_object(90,16,SKULL_SIZE,SKULL_SIZE,(const char *)skull);
		if(difficulty == 1){
			display_object(28,6,PLAYER_SIZE,PLAYER_SIZE,(const char *)crown);
		}
		else{
			display_object(92,6,PLAYER_SIZE,PLAYER_SIZE,(const char *)crown);
		}
		display_update();
		if(btn_data&4){
			wait_for_button_release();
			if(difficulty==1)
			difficulty = 2;
			else
			difficulty = 1;
		}
		if(btn_data&8){
			wait_for_button_release();
			break;
		}

	}
	// Returns binary number, 00 = singleplayer easy
	//                               11 = 2 player hard
	return((players-1+((difficulty-1)*2)));

}
