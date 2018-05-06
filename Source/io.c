/*
io.c
Created 2018-02-09 by Antonio Morales and Pebo Hamza
*/

#include <pic32mx.h>
#include "project.h"

/*
Initializes all the switches and buttons to inputs
*/

void input_init(void){
  TRISD |= 0x0fe0;
  TRISF|=0x2; //btn1
}


int getsw (void){
  int switches_data = (PORTD  & 0x00000f00)>>8;
  return switches_data;
}

int getbtns(void){
  int btn_data=((PORTD & 0x000000e0)>>4)|((PORTF & 0x00000002)>>1);
  return btn_data;
}
