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
		if ((state != 1) || (pressed == 1)) { 	// if not state 1
			output_handler(); 					// write to LEDS 
		} 										// else, return
		
	}
}
