#include "project.h"
#include "pic32mx.h"

#define MAX_BLOCK 3 //number of blocks
#define START_X 128 //Outside display
#define SPACE 60  //15 + actual space

int block_coordinates[MAX_BLOCK][2]; // = {{0,0},{0,0},{0,0}};

int global_random_counter = 0;

char controll_matrix [40][32];

//[x][y]
const char block [15][16] = {
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1},
{1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1},
{1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1},
{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1},
{1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1},
{1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,1},
{1,1,0,0,0,1,1,1,0,1,1,1,1,1,1,1},
{1,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void move_left(void){
  int b;
  for (b = 0; b < MAX_BLOCK; b++){
    block_coordinates[b][0]--;
    if(block_coordinates[b][0]<=-52){ //when out of the screen reset, calculated to look smooth
      block_coordinates[b][0]=128;
      block_coordinates[b][1]= global_random_counter %16; //take something random
    }
  }
}

/*
Gets called repeatedly from interrupts.
Counts a global variable for a random-generator purpose...
*/
void counter(void){
  global_random_counter++;
  if (global_random_counter>=100000){
    global_random_counter = 0;
  }
}

/*
Initializes the blocks starting positions
*/
void block_init(void){
  crash_zero(); //Important to clear earlier "control-matrix"
  int b;
  int coordinate;
  int start_y = 0;
  for(b = 0; b < MAX_BLOCK; b++){
    block_coordinates[b][0] = SPACE*b + START_X;
    block_coordinates[b][1] = start_y;
    start_y+=5;
  }
  for (coordinate = 0; b < MAX_BLOCK; b++)
    display_object(block_coordinates[b][0], block_coordinates[b][1], BLOCK_WIDTH, BLOCK_HEIGHT, (const char *)block);
}

/*
Updates the display-matrix with the currently position of the blocks
*/
void block_update(void){
  int i;
  for(i = 0; i < MAX_BLOCK; i++){
    display_object(block_coordinates[i][0], block_coordinates[i][1], BLOCK_WIDTH, BLOCK_HEIGHT, (const char *)block);
  }
}



/*
Functions to build our "control-matrix" and "control-blocks" that will be
used to check if there has been a crash.
*/
void turn_on_controll_pixel (int x, int y, int on_off){
	if(x >= 40 || x < 0 || y >= MAX_Y || y < 0) //if it is out of range
		return;

	controll_matrix[x][y]=on_off; //turn on/off
}


void build_controll_block (int pos_x, int pos_y){
	int x,y;
	for (x = 0; x < BLOCK_WIDTH; x++)
		for (y = 0; y < BLOCK_HEIGHT; y++)
			if(block[x][y])
				turn_on_controll_pixel(pos_x + x, pos_y + y, 1);
}



/*Compares the "player-area" of the display matrix with the control-matrix
of the same size as the "player-area" to check if there are some commmon pixels,
if that is the case it means a collision has occured.
*/
void crash_zero (void){
	int x,y;
	for (x = 0; x < 40; x++)
		for (y = 0; y < 32; y++)
			controll_matrix[x][y] = 0;
}

int crash_check (void){
  int b;
  for(b = 0; b < MAX_BLOCK; b++){
    if(block_coordinates[b][0]<=40 && block_coordinates[b][0]>0){
      build_controll_block(block_coordinates[b][0],block_coordinates[b][1]);
    }
  }
  /*Now we have built our "control-matrix".
  We must now, probably in main, display_zero and display_player(s) to
  compare the two matrix within the player-area to see if there are any crashes.
  */
  int x, y;
  for(x = 0; x < 40; x++ ){
    for (y = 0; y < 32; y++){
      if(display[x][y]&controll_matrix[x][y]){
        return 1;
      }
    }
  }
crash_zero(); //reset for next check
return 0;
}
