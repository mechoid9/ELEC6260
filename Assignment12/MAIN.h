/**********************************************/
/*   Brian Arnberg                            */
/*    main.h: primary include file for this   */
/*            project                         */
/**********************************************/

#ifndef __MAIN_H
#define __MAIN_H
#define EXTI_Line0       ((uint32_t)0x00001)     /*!< External interrupt line 0 */
#define EXTI_PortSourceGPIOA       ((uint8_t)0x00)
#define EXTI_PinSource0            ((uint8_t)0x00)
#define amplitude 2048 		// amplitude of the sinusoidal tone

// Include the following header files to allow for access to standard libraries. :)
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include "stm32f4_discovery_audio_codec.h"
#include "stm32f4xx_i2c.h"

/*  General Globals  */
extern volatile uint32_t state;		// initialize state
extern volatile uint32_t step;		// initialize step
extern volatile uint32_t pressed;	// initialize pressed
extern uint32_t ccw[4];			// initialize counter clockwise array
extern uint32_t cw[4];			// initialize clockwise array
extern uint32_t msTicks;     		/* counts 1ms timeTicks       */

/*  Audio Globals    */
extern volatile uint32_t audioSample;	// value of audio sample (16-bit) (32 samples long)
extern uint32_t tickNumber;		// current sampling tick
extern uint32_t tickIncrement;		// how many ticks till next sample change
extern double toneFrequency[8];	// frequency for the tone
extern unsigned char tone;		// current tone
extern uint16_t sampleRate;		// sample rate for the tone 
extern unsigned char play;			// Current playing state: 0 = stopped, 1 = playing
void Timer_Computations ( void ); // function executed by timer interrupt



extern void EXTI0_IRQHandler(void); 	// External Interupt Handler
extern void input_handler(void);	// declare input handler
extern void enable_leds(int PIN); 	// declare enable_leds (in output_handler.s)
extern void clear_leds(int PIN);	// declare disable_leds (in output_handler.s)
extern uint32_t sinApprox(void);

#endif

