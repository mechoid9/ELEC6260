/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #6 - System Interrupts                  */
/*  tick_timer.c                                       */
/*******************************************************/
/*  SysTick_Handler - once every ms                    */
/*     - calls SVC_Handler every 500ms                 */
/*  SVC_Handler - called by SysTick                    */
/*     - handles output based on global variables      */
/*  EXTI0_IRQHandler - triggered by a button press     */
/*     - configured in main.c                          */
/*     - executes the input handler (self-debounced)   */
/*     - clears pending status                         */
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
		__asm("SVC 0");							// Supervisor Call
	}
}

void EXTI0_IRQHandler  (void) { // This simply calls the input_handler
		input_handler();
	  EXTI->PR = EXTI_Line0;
	}

void SVC_Handler(void) {
	int i;
	if ((pressed == 1) || (step == 4)) { // if I've pressed a button or reached max step
			step = 0;	// reset the step counter
			pressed = 0; 	// clear pressed
			for (i = 12; i <= 15; i++) { // clear the 4 leds
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
