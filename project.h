/*
Here is all the functions, constants, global variables etc for this project
*/

#ifndef _PROJECT_H
#define _PROJECT_H
/*****************************DISPLAY******************************************/

//The display matrix (128 * 32)
#define MAX_Y 32
#define MAX_X 128
char display [MAX_X][MAX_Y];

void quick_wait(int c);
void display_init(void);
void display_pixel(int x, int y, int on_off);
void display_zero(void);
void display_update(void);

void display_object (int pos_x, int pos_y, int x, int y, const char *pixel_array);//display-function

/******************************************************************************/

/*******************************INPUTS*****************************************/

void input_init(void);
int getsw(void);
int getbtns(void);

/******************************************************************************/

/************************************TIMER*************************************/

void timer_init(void);
void reset_timer(void);
void reset_timecount(void);
void timer_hold(int ms);

/******************************************************************************/

/********************************INTERRUPTS************************************/

void interrupts_init(void);
void enable_interrupt(void);
void interrupts_on(void);
void interrupts_off(void);
void put_speed(int new_speed);


/******************************************************************************/

/*****************************LEDS*********************************************/

void led_init(void);
void led_count(void);
void led_reset(void);
void second_reset(void);

/******************************************************************************/

/**************************PLAYER**********************************************/
#define PLAYER_SIZE 8
#define PLAYER1_X 10
#define PLAYER2_X 30
extern const char player_object[PLAYER_SIZE][PLAYER_SIZE];

extern int player1_y;
extern int player2_y;
void up_1 (void);
void down_1 (void);
void up_2 (void);
void down_2 (void);

void move(void);
void players_init(void);

/************************************BLOCKS************************************/
#define BLOCK_HEIGHT 16
#define BLOCK_WIDTH  15

extern const char block [BLOCK_WIDTH][BLOCK_HEIGHT];
void block_init(void);
void move_left(void);
void counter(void);
void block_update(void);


void crash_zero(void);
int crash_check (void);

/**********************************MENU****************************************/

int menu_init(void);

/******************************************************************************/

#endif
