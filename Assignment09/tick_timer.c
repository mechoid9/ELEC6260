/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #5 - System Tick Timer                  */
/*  tick_timer.c                                       */
/*******************************************************/
/*  Activates once every half second                   */
/*  Calls output_handler (iff LEDS should change)      */
/*******************************************************/
/*  ``state'' is a global variable.                    */
/*******************************************************/
#include "STM32F4xx.h"
#include "MAIN.h"

volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */

void SysTick_Handler ( void ) {
	msTicks++;
	if (msTicks == 500) {
	msTicks = 0; 								// reset the msTicks
		if ((pressed == 1) || (step == 4)) { // if I've pressed a button or reached max step
			step = 0;	// reset the step counter
			pressed = 0; 	// clear pressed
			for (int i = 12; i <= 15; i++) { // clear the 4 leds
				clear_leds(i);
			}
	       	} else if (state == 2)  {	//counter clockwise output
			enable_leds(ccw[step]); //output to appropriate LED
			step++; //increment step
		} else if (state == 3) { //clockwise output
			enable_leds(cw[step]); 	//output to appropriate LED
			step++; //increment step
		}
	}
}
