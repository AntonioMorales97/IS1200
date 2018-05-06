#include <pic32mx.h>
#include "project.h"
int timeoutcount = 0; //Counter used within the interrupt-function
int speed = 1000; //standard one second (because of prescale settings)
int timeoutcount_2 = 0; //Other counter also used with interrupts, for another purpose

//Interrupts comes from timer2

void interrupts_init(void){
  IPCSET(2)=0x4; //T2IP bits 4-2, this means prio lvl 1. Set prio.
  enable_interrupt(); //call the asm function in enable_interrupt.S
}

void interrupts_off (void){
  IECCLR(0)=0x100; //Turn off by setting bit 8 in T2IE to 0. It enables interrupts from timer2. See datasheet
}

void interrupts_on(void){
  IECSET(0)=0x100; //Turn on by setting the bit to 1.
}

// Interrupts, 1000 per second
void user_isr( void ){
  counter(); //A function in block.c
  timeoutcount++;
  timeoutcount_2++;
  IFSCLR(0)=0x100; //clear the timer2 timeoutflag!
  if(timeoutcount == speed){
    timeoutcount = 0; //reset
    move_left(); //Moves the blocks to the left
  }

  if(timeoutcount_2 == 1000){ //for count leds/second
    timeoutcount_2 = 0;
    led_count(); //function in led.c
  }

}

//Change the speed-variable (ms)
void put_speed (int new_speed){
  timeoutcount = 0;
  speed = new_speed;
}
