#include "project.h"
#include "pic32mx.h"
//int settings is a binary number, 00 = single player easy, 11 = two player hard, 10 = one player hard, 01 = two player easy
int settings;

//Initializes everything that needs to be initialized
void init (void){
  timer_init();
  led_init();
  interrupts_init();
  display_init();
  input_init();
  block_init();
  players_init();
}

//The name of the function pretty much says it. Called repeatedly from main
int play_game(void){

    while((getsw())&1){ //Pause the game
      interrupts_off();
    }

    interrupts_on(); //start moving the blocks and count seconds...
    display_zero(); //Reset the display matrix
    timer_hold(20); //Adjust speed of players, hold the PC for 20 ms.
    move();

    display_object(PLAYER1_X, player1_y, PLAYER_SIZE, PLAYER_SIZE, (const char *)player_object);
	if (settings&1) //If multiplayer-mode is enabled
		display_object(PLAYER2_X, player2_y, PLAYER_SIZE, PLAYER_SIZE, (const char *)player_object);

    char check = crash_check(); //Check for crash
    if (check){
      interrupts_off(); //otherwise the blocks continues!
      second_reset(); //reset second-counter
      while(~(getsw())&8){  //Pull up switch 4 to exit and play again
        if ((getsw())&4){   //Pull up switch 3 to exit and go to menu
	         settings = menu_init();
	         return 0;
	      }
	    }
      timer_hold(1000);
      return 0;
    }

    block_update();
    display_update();
    counter(); //count the global integer variable for random-purpose. Already called from interrupts, test remove!?
    return 1;
}



int main(){
  init();
  settings = menu_init(); //Decide mode and difficulty
  while(1){
    int loop = 1;
    block_init(); 
    players_init();
    display_zero();
    display_update();
    timer_hold(1000); //wait 1 second
    interrupts_on(); //begin
	  put_speed(25);
	  if(settings&2)
      put_speed(20); //If lvl-hard is chosen
    while(loop){
      loop = play_game(); //Keep playing until you crash!
    }
    display_zero();
    display_update();
  }
  return 0; //Will never be reached.
}
