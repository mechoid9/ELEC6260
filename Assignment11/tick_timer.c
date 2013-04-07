/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #7 - System Interrupts                  */
/*  tick_timer.c                                       */
/*******************************************************/
/*  SysTick_Handler - once every 0.05ms                */
/*                                                     */
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
#include <math.h> 


uint32_t msTicks;                      /* counts 1ms timeTicks       */
volatile unsigned int audioSample = 0;			// holds the audio sample
unsigned int tone = 0;					// initialize to NULL tone
uint16_t toneFrequency[9] = {0, 131, 147, 165, 175, 195, 220, 247, 262};	// tone frequency (rounded) for C-Major scale
unsigned char yes = 0; 			// should I update the audioSample?
double MOD;				// modify the argument to the sine wave
double PI = 3.14159;
double argument, result;	
char toggle;
	
void SysTick_Handler ( void ) {

	msTicks++;


	if (yes != 0) {
		argument = MOD*msTicks;
		result = 32768 + 32768*sin(argument);
		audioSample = (uint16_t)result;
	}

	if (msTicks == ((sampleRate - 1))) {
		msTicks = 0; 		// reset the msTicks
		if (toggle == 0) toggle = 1;
		else toggle = 0;
		if (pressed == 1) {
			yes = 1;
			tone = (tone + 1)%9;
			MOD = 2*PI*toneFrequency[tone]/sampleRate;
			pressed = 0;
		}
	
		__asm("SVC 0");		// Supervisor Call
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
