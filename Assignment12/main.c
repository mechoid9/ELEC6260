/*******************************************************/
/* Brian Arnberg                                       */
/* Problem Set #7 - Main Program                       */
/*  main.c                                             */
/*******************************************************/
/*  Audio Tone Generator                               */
/*   pressing a button cycles through the tones        */
/*******************************************************/
/*  global variables defined in ``main.h''             */
/*******************************************************/
#include <stdio.h>
#include "STM32F4xx.h"
#include "main.h"
#include "stm32f4_discovery_audio_codec.h"
#include "stm32f4xx_i2c.h"

volatile uint32_t state;								/* keep track of the system state             */
volatile uint32_t pressed;							/* keep track of whether a button was pressed */
volatile uint32_t step;									/* keep track of the system step              */
uint32_t ccw[4] = {13,12,15,14};        /* counter clock-wise pin order               */
uint32_t cw[4] = {13,14,15,12};         /* clock-wise pin order                       */



uint32_t tickNumber;			// current sampling tick
uint16_t sampleRate = 20000;			// sample rate for the tone (20kHz)


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
 **********************************************/
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line)
{
	EXTI->SWIER |= EXTI_Line;
}

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAC_DHR12R2_ADDRESS    0x40007414
#define DAC_DHR8R1_ADDRESS     0x40007410

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_InitTypeDef  DAC_InitStructure;

/**
  * @brief  TIM6 Configuration
  * @note   TIM6 configuration is based on CPU @168MHz and APB1 @84MHz
  * @note   TIM6 Update event occurs each 84MHz/(9+1) = 8.4 MHz     
  * @param  None
  * @retval None
  */
void TIM6_Config(void)
{
  	
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	  
	
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = 419;  //  8.4MHz/20KHz = (419 + 1)
  TIM_TimeBaseStructure.TIM_Prescaler = 9; //  84MHz/(9+1) = 8.4MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  
  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
	



  /* Enable the TIM3 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM6->DIER |= TIM_DIER_UIE;                         // Enable interrupt on update event
		
		
}


/**
  * @brief  DAC Channel1 Configuration
  * @param  None
  * @retval None
  */
void DAC_Ch1_Config(void)
{
  
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); // enable the IO port A clock
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_DACEN);   // enable the DAC clock
	SET_BIT(GPIOA->MODER, GPIO_MODER_MODER4);   // PA4 = Analog
	SET_BIT(DAC->CR, DAC_CR_EN1);
	
}

/**
* @brief  Get next data sample callback
* @param  None
* @retval Next data sample to be sent
*/
uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
  return 0;
}


void CODEC_Init (void) {
	I2C_InitTypeDef 	I2C_InitType;
	// Clock Turn on

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_SPI3, ENABLE);
	RCC_PLLI2SCmd(ENABLE);
	
I2C_InitType.I2C_ClockSpeed = 100000;
I2C_InitType.I2C_Mode = I2C_Mode_I2C;
I2C_InitType.I2C_OwnAddress1 = 99;
I2C_InitType.I2C_Ack = I2C_Ack_Enable;
I2C_InitType.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
I2C_InitType.I2C_DutyCycle = I2C_DutyCycle_2;
	
	

I2C_Init(I2C1, &I2C_InitType);   //initialize the I2C peripheral ...
I2C_Cmd(I2C1, ENABLE);  
	
}

/*--------------------------------
Callbacks implementation:
the callbacks prototypes are defined in the stm324xg_eval_audio_codec.h file
and their implementation should be done in the user code if they are needed.
Below some examples of callback implementations.
--------------------------------------------------------*/
/**
* @brief  Calculates the remaining file size and new position of the pointer.
* @param  None
* @retval None
*/
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
  /* Calculate the remaining audio data in the file and the new size 
  for the DMA transfer. If the Audio files size is less than the DMA max 
  data transfer size, so there is no calculation to be done, just restart 
  from the beginning of the file ... */
  /* Check if the end of file has been reached */
  
#ifdef AUDIO_MAL_MODE_NORMAL  
  
#if defined MEDIA_IntFLASH

#if defined PLAY_REPEAT_OFF
  LED_Toggle = 4;
  RepeatState = 1;
  EVAL_AUDIO_Stop(CODEC_PDWN_HW);
#else
  /* Replay from the beginning */
  AudioFlashPlay((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS),AUDIO_FILE_SZE,AUIDO_START_ADDRESS);
#endif  
  
#elif defined MEDIA_USB_KEY  
  XferCplt = 1;
  if (WaveDataLength) WaveDataLength -= _MAX_SS;
  if (WaveDataLength < _MAX_SS) WaveDataLength = 0;
    
#endif 
    
#else /* #ifdef AUDIO_MAL_MODE_CIRCULAR */
  
  
#endif /* AUDIO_MAL_MODE_CIRCULAR */
}


/***********************************************
 *  main function call!!!!
 **********************************************/
int main (void) {                     
//	int i;
  /* Preconfiguration before using DAC----------------------------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DMA1 clock and GPIOA clock enable (to be used with DAC) */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);

  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* DAC channel 1 (DAC_OUT1 = PA.4) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// initialize some variables
	state = 1; 				// Initialize State
	pressed = 0;			// Make sure pressed is clear

	/* Initialize the LEDS and the buttons */
	LED_Init();
	BTN_Init();    
	EXTILine0_Config();
	EXTI_GenerateSWInterrupt(EXTI_Line0);
	
	/* Initialize Timer 6 */
	TIM6_Config();
	

	/* Initialize the DAC */
	DAC_DeInit(); // clear DAC
	DAC_Ch1_Config(); // configure DAC channel 1 (PA4) to access memory associated with audioSample  

	

	
	//CODEC_Init(); //not called because not needed. 
			
	for (;;) { 			// loop forever

		
	}
}
