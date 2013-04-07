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
volatile unsigned int audioSample;			// holds the audio sample
double degree_tick;			// degrees per tick
double toneFrequency[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};	// tone frequency for C-Major scale
double tick_tone;				// number of ticks per period for the current tone
unsigned char tone = 0;					// initialize to lower C
unsigned char play = 0;			// Current playing state: 0 = stopped, 1 = playing
unsigned char lastSecond;
uint32_t nextSecond;
char debounced = 1;
uint32_t nextBounce;
uint32_t watchAudio;


char toggle;

/* Sine approximation algorithm  */
/* returns an approximate sinusoid 
 * 	with a DC offset equal to the amplitude
 */
uint32_t sinApprox( void ) {
	static char up;
	double x, result;
	//int i, h;
	int h;
	//i = msTicks % (uint32_t)(tick_tone);
	h = msTicks % (uint32_t)(tick_tone/2);
	x = degree_tick * h;
	result = (4*x*(180-x))/(40500 - x * (180 - x));
	if ((h == 0) && (up != 1)) { up = 1; }
  else if (( h == 0 ) && (up == 1)) { up = 0; }
	if (up) { 
		result = amplitude + amplitude * result;
	}	else {
		result = amplitude - amplitude * result;
	}
	return result;
}

/* blink(int LED)
 *  blink the current LED based on the sampling 
 *  rate of the current tone
 */
void blink(int LED) {
	static char toggle;
	unsigned int i;
	i = 1+tone;
	i = sampleRate/i;
	i = msTicks % i;
	if ((i == 0) && (toggle!= 0)){
		toggle = 0;
		enable_leds(LED);
	}
	else if ((i == 0) && (toggle == 0)) {
		toggle = 1;
		clear_leds(LED);
	}
}

/* display(int tone)
 *  display the current tone to the top three output LED's
 */
void displayTone(char tone) {
	if (tone & 0x1) enable_leds(LED5+12);
	else clear_leds(LED5+12);
	if (tone & 0x2) enable_leds(LED3+12);
	else clear_leds(LED3+12);
	if (tone & 0x4) enable_leds(LED4+12);
	else clear_leds(LED4+12);
}
	

/* The timer interrupt (or SystemTick_Handler) calls this function.*/
/* Timer_Computations - void function handles some timer logic */
void Timer_Computations ( void ) {
	msTicks++; //increment current tick
	/* if play is true, then play truly */
	if (play == 1) {
		audioSample = sinApprox(); //calculate current sample
		DAC->DHR12R1 = audioSample; //output current sample to DAC
		blink(LED6+12); //call blink function
	} else if (play == 0) {
		audioSample = amplitude; //set amplitude to middle value
		DAC->DHR12R1 = audioSample; //output current sample to DAC
		clear_leds(LED6+12); //make sure LED6 is clear/(no longer blinking)
	}
	
	/* check to see if the 1-second time is up */
	if ((msTicks == nextSecond) && (lastSecond == 1)) {
		lastSecond = 0;
	}
	
	/* handle debouncing */
	if (msTicks%(sampleRate/8) == 0) { //set debounced to 1 every 1/8th second
		debounced = 1;
	}
	
	/* if a button was pressed, decide how to react */
	if (pressed == 1) {
		/* How do I change state? */
		if (lastSecond == 1) {
			play = 0;
			clear_leds(LED6+12);
		}
		else if ( play != 1 ) {
			play = 1;
			tick_tone = sampleRate/toneFrequency[tone];
			degree_tick = 360/tick_tone;
			lastSecond = 1;
			nextSecond = msTicks + sampleRate;
		}
		else if ( play == 1 ) {
			tone = (tone + 1)%8;
			tick_tone = sampleRate/toneFrequency[tone];
			degree_tick = 360/tick_tone;
			displayTone(tone);
			lastSecond = 1;
			nextSecond = msTicks + sampleRate;
		}
		/* Based on the state change, what do I do to the audio player? */
		if (play == 0x01) { //if the play status is true, initialize audio output
			EVAL_AUDIO_SetAudioInterface(AUDIO_INTERFACE_DAC);
			EVAL_AUDIO_Init(OUTPUT_DEVICE_HEADPHONE, 50, sampleRate);
			EVAL_AUDIO_Play((uint16_t *)audioSample, 1); //possibly not needed
		} else { //otherwise, kill audio output 
			EVAL_AUDIO_Stop(CODEC_PDWN_SW);
		}
		pressed = 0;
	}
	
}

/* Timer Channel 6 interrupt handler  */
void TIM6_DAC_IRQHandler(void) {
	if(TIM6->SR)
		Timer_Computations(); // call the logic that this timer channel should execute
	TIM6->SR &= ~1;   // Interrupt has been handled
}

/* Input Handler - Called when the user button is pressed  */
void EXTI0_IRQHandler  (void) { // This simply calls the input_handler
	if (debounced) { //if the button has been debounced...	
		input_handler(); //check the button
		nextBounce = msTicks + (sampleRate/8); //set the nextBounce parameter
		debounced = 0; //clear ``debounced'' so that input_handler isn't called again until the system has debounced
	} 
	EXTI->PR = EXTI_Line0; //interrupt handled
}

// Do nothing during systick...
void SysTick_Handler (void) {
}

/* Supervisor call - SVC_Handler */
void SVC_Handler(void) {
// do nothing!	
}
