/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #6 - Main Program                       */
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
volatile uint32_t state;								/* keep track of the system state             */
volatile uint32_t pressed;							/* keep track of whether a button was pressed */
volatile uint32_t step;									/* keep track of the system step              */
uint32_t ccw[4] = {13,12,15,14};        /* counter clock-wise pin order               */
uint32_t cw[4] = {13,14,15,12};         /* clock-wise pin order                       */

/**   From stm32f4xx_syscfg.c
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  EXTI_PortSourceGPIOx : selects the GPIO port to be used as source for
  *          EXTI lines where x can be (A..I).
  * @param  EXTI_PinSourcex: specifies the EXTI line to be configured.
  *           This parameter can be EXTI_PinSourcex where x can be (0..15, except
  *           for EXTI_PortSourceGPIOI x can be (0..11).
  * @retval None
  */
void SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
{
  uint32_t tmp = 0x00;

  tmp = ((uint32_t)0x0F) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03));
  SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] &= ~tmp;
  SYSCFG->EXTICR[EXTI_PinSourcex >> 0x02] |= (((uint32_t)EXTI_PortSourceGPIOx) << (0x04 * (EXTI_PinSourcex & (uint8_t)0x03)));
}

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


void EXTILine0_Config(void)
{
	uint32_t tmp = 0;
	uint8_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
	/* Connect EXTI Line0 to PA0 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	
	tmp = (uint32_t)EXTI_BASE;
	
	/* Clear EXTI line configuration */
	EXTI->IMR &= EXTI_Line0;
	EXTI->EMR &= EXTI_Line0;
	
	// EXTI_Mode_Interrupt = 0x00  
	tmp += 0x00;
	*(__IO uint32_t *) tmp |= EXTI_Line0;
  
	/* Clear Rising Falling edge configuration */
	EXTI->RTSR &= EXTI_Line0;
	EXTI->FTSR &= EXTI_Line0;
	
	// EXTI_Trigger_Rising
	tmp = (uint32_t)EXTI_BASE;
 	tmp += 0x08;
	*(__IO uint32_t *) tmp |= EXTI_Line0;
	 
	 /* Enable and set EXTI Line0 Interrupt to the lowest priority */
	
	/* Compute the Corresponding IRQ Priority --------------------------------*/    
	tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
	tmppre = (0x4 - tmppriority);
	tmpsub = tmpsub >> tmppriority;
		
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	
	tmppriority = 0x01 << tmppre;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01; 
	tmppriority |=  (uint8_t)(0x01 & tmpsub);
        
	tmppriority = tmppriority << 0x04;
        
	//NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC->IP[EXTI0_IRQn] = tmppriority;
    
	/* Enable the Selected IRQ Channels --------------------------------------*/
	NVIC->ISER[EXTI0_IRQn >> 0x05] =
	(uint32_t)0x01 << (EXTI0_IRQn & (uint8_t)0x1F);
}

/***********************************************
 * Generates SWI on particular line
 ***********************************************/
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line)
{
	EXTI->SWIER |= EXTI_Line;
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
	EXTILine0_Config();
	EXTI_GenerateSWInterrupt(EXTI_Line0);
	
	
	for (;;) { 			// loop forever
	}
}

