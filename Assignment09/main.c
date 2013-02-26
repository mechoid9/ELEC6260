/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #5 - Main Program                       */
/*  main.c                                             */
/*******************************************************/
/*  Continuous Loop                                    */
/*  Calls input_handler each loop                      */
/*******************************************************/
/*  ``state'' is a global variable.                    */
/*  ``pressed'' is also a global variable              */
/*******************************************************/
#include <stdio.h>
#include "STM32F4xx.h"
#include "main.h"
volatile uint32_t state;		/* keep track of the system state             */
volatile uint32_t pressed;		/* keep track of whether a button was pressed */
volatile uint32_t step;			/* keep track of the system step              */
uint32_t ccw[4] = {13,12,15,14};        /* counter clock-wise pin order               */
uint32_t cw[4] = {13,14,15,12};         /* clock-wise pin order                       */

/*----------------------------------------------------------------------------
  Function that initializes Button pins
 *----------------------------------------------------------------------------*/
void BTN_Init(void) {

  RCC->AHB1ENR  |= ((1UL <<  0) );              /* Enable GPIOA clock         */

  GPIOA->MODER    &= ~((3UL << 2*0)  );         /* PA.0 is input              */
  GPIOA->OSPEEDR  &= ~((3UL << 2*0)  );         /* PA.0 is 50MHz Fast Speed   */
  GPIOA->OSPEEDR  |=  ((2UL << 2*0)  ); 
  GPIOA->PUPDR    &= ~((3UL << 2*0)  );         /* PA.0 is no Pull up         */
}

/*----------------------------------------------------------------------------
  initialize LED Pins
 *----------------------------------------------------------------------------*/
void LED_Init (void) {

  RCC->AHB1ENR  |= ((1UL <<  3) );         /* Enable GPIOD clock                */

  GPIOD->MODER    &= ~((3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PD.12..15 is output               */
  GPIOD->MODER    |=  ((1UL << 2*12) |
                       (1UL << 2*13) | 
                       (1UL << 2*14) | 
                       (1UL << 2*15)  ); 
  GPIOD->OTYPER   &= ~((1UL <<   12) |
                       (1UL <<   13) |
                       (1UL <<   14) |
                       (1UL <<   15)  );   /* PD.12..15 is output Push-Pull     */
  GPIOD->OSPEEDR  &= ~((3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PD.12..15 is 50MHz Fast Speed     */
  GPIOD->OSPEEDR  |=  ((2UL << 2*12) |
                       (2UL << 2*13) | 
                       (2UL << 2*14) | 
                       (2UL << 2*15)  ); 
  GPIOD->PUPDR    &= ~((3UL << 2*12) |
                       (3UL << 2*13) |
                       (3UL << 2*14) |
                       (3UL << 2*15)  );   /* PD.12..15 is Pull up              */
  GPIOD->PUPDR    |=  ((1UL << 2*12) |
                       (1UL << 2*13) | 
                       (1UL << 2*14) | 
                       (1UL << 2*15)  ); 
}


int main (void) {                     
	state = 1; 				// Initialize State
	pressed = 0;			// Make sure pressed is clear
	
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000 )) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
	
	/* Initialize the LEDS and the buttons */
	LED_Init();
	BTN_Init();    
	
	for (;;) { 			// loop forever
	input_handler();		// call input_handler
	}
}

