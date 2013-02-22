/**********************************************/
/*   Brian Arnberg                            */
/*    main.h: primary include file for this   */
/*            project                         */
/**********************************************/

#ifndef __MAIN_H
#define __MAIN_H

extern volatile uint32_t state; 					// initialize state
extern volatile uint32_t pressed;					// initialize pressed
extern void input_handler(void);					// declare input handler
extern void output_handler(void);					// declare output handler
extern volatile uint32_t msTicks;                      /* counts 1ms timeTicks       */


#endif
