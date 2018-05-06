#include "project.h"

int player1_y = 16; //Y-coordinate
int player2_y = 16;

/*
    E
	N		S
		W
*/
//This is our cute player-object
const char player_object[PLAYER_SIZE][PLAYER_SIZE]={
  {1,1,1,1,1,1,1,1},
	{1,0,0,0,1,1,1,1},
	{1,0,0,0,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,0,0,0,1,1,1,1},
	{1,0,0,0,1,1,1,1},
	{1,1,1,1,1,1,1,1}
};

//Initialize start y-positions for the player
void players_init(void){
  player1_y = 16;
  player2_y = 16;
}


void up_1 (void){
  if (player1_y <= 0 ) //MAX height
    return;
  player1_y--; //move upp
}

void down_1 (void){
  if (player1_y >=24) //32-8, lowest point. If 8x8 player-object
    return;
  player1_y++;
}

void up_2 (void){
  if (player2_y <= 0 )
    return;
  player2_y--;
}

void down_2 (void){
  if (player2_y >=24)
    return;
  player2_y++;
}

void move (void){
  int btn_data = getbtns();
  if(btn_data&1){
    up_2();
  }
  if(btn_data&2){
    down_2();
  }
  if(btn_data&4){

    up_1();
  }
  if(btn_data&8){
    down_1();
  }
}
