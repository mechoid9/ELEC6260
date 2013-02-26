/**********************************************/
/*   Brian Arnberg                            */
/*    main.h: primary include file for this   */
/*            project                         */
/**********************************************/

#ifndef __MAIN_H
#define __MAIN_H

extern volatile uint32_t state;				// initialize state
extern volatile uint32_t step;				// initialize step
extern volatile uint32_t pressed;			// initialize pressed
extern uint32_t ccw[4];					// initialize counter clockwise array
extern uint32_t cw[4];					// initialize clockwise array
extern void input_handler(void);			// declare input handler
extern void enable_leds(int PIN); 			// declare enable_leds (in output_handler.s)
extern void clear_leds(int PIN);			// declare disable_leds (in output_handler.s)
extern volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */


#endif
