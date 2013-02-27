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

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

extern volatile uint32_t state;				// initialize state
extern volatile uint32_t step;				// initialize step
extern volatile uint32_t pressed;			// initialize pressed
extern uint32_t ccw[4];					// initialize counter clockwise array
extern uint32_t cw[4];					// initialize clockwise array
extern volatile uint32_t msTicks;     /* counts 1ms timeTicks       */


extern void EXTI0_IRQHandler(void); 	// External Interupt Handler
extern void input_handler(void);	// declare input handler
extern void enable_leds(int PIN); 	// declare enable_leds (in output_handler.s)
extern void clear_leds(int PIN);	// declare disable_leds (in output_handler.s)

#endif
