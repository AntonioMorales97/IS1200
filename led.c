#include "project.h"
#include <pic32mx.h>

int record = 0;
int second_counter = 0;

//Initializes the leds to be outputs
void led_init(void){
  TRISECLR = 0x00FF;
  PORTE=0;
}

//called from interrupts, 1 time per second, counts only if record beaten
void led_count(void){
  second_counter++;
  if(second_counter>=record){
    record = second_counter;
    PORTE = record; //Output to the LEDs
  }
}

void second_reset(void){
  second_counter = 0;
}

void led_reset(void){
  PORTE=0;
}
