#include <pic32mx.h>
#include "project.h"

#define TMR4PERIOD ((80000000 / 256) / 1000) //prescale 1:256 and 1/1000 of a second
#if TMR4PERIOD > 0xffff //just in case
#error "Period too big."
#endif
#define TMR4FLAG 0x10000
#define TMR2FLAG 0x100

void timer_init(void){
  T4CONSET = 0x70;//prescale 1:256, bit 6-4
  PR4 = TMR4PERIOD; //set the period-register
  IFSCLR(0) = TMR4FLAG; //clear the timer4interuptflag!
  TMR4 = 0; //reset timer
  T4CONSET = 0x8000; //set timer enable (bit 15 should be 1)

  //Do the same for timer2...
  T2CONSET = 0x70;
  PR2 = TMR4PERIOD;
  IFSCLR(0) = TMR2FLAG;
  TMR2 = 0;
  T2CONSET = 0x8000;

  return;
}

void reset_timer(void){
  TMR2=0;
  TMR4=0;
}

//Holds for x ms
void timer_hold(int ms){
  int timecount = 0;
  IFSCLR(0)=TMR4FLAG;
  while(timecount<(ms)){
    if (IFS(0)&TMR4FLAG){ //checks if time out
      timecount++;
      IFSCLR(0)=TMR4FLAG; //clr timer4 timeoutflag here. if not reset => rly fast, always done..
    }
  }
}
