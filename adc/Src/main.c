#include <stdint.h>
#include "stm32f1xx.h"

volatile uint16_t adc_data[3];

void configTimer3();
void configGPIO();
void configADC (void);
void enableADC (void);
void startADC(void);
void configDMA ();

int main(void)
{
	configGPIO();
	configTimer3();
	configADC();
	enableADC();
	configDMA();
	startADC();

	while (1)
	{

		TIM3->CCR1 = adc_data[0] / 42;  // Ajusta o PWM para CH1
		TIM3->CCR2 = adc_data[1] / 42;  // Ajusta o PWM para CH2
		TIM3->CCR3 = adc_data[2] / 42;  // Ajusta o PWM para CH3


	}
}

void configTimer3()
{
	//Clock AFIO
	RCC->APB2ENR |= (1 << 0);
	//CH1 - A6, CH2 - A7, CH3 - B0
	AFIO->MAPR &= (0x00 << 10);

	//clock TIMER3
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->PSC = 0;
	TIM3->ARR = 100;

	TIM3->CCMR1 = 0x6060;
	TIM3->CCMR2 = 0x0060;
	TIM3->CCER = 0x0111;
	TIM3->CR1  |= (1 << 0);
}

void configGPIO()
{
	RCC->APB2ENR |= (1 << 2);
	RCC->APB2ENR |= (1 << 3);

	GPIOA->CRL &= 0xFFFFF000;
	GPIOA->CRL &= 0x00FFFFFF;
	GPIOA->CRL |= 0xBB000000;

	GPIOB->CRL &= 0xFFFFFFF0;
	GPIOB->CRL |= 0x0000000B;
}

void configADC (void)
{
	RCC->APB2ENR |= 1<<9;  // enable ADC1 clock

	ADC1->CR1 = (1<<8);    // SCAN mode enabled

	ADC1->CR2 = (1<<1);     // enable continuous conversion mode

	ADC1->CR2 |= (7<<17);  // External Event selection pointed to SWSTART bit

	ADC1->CR2 &= ~(1<<11);   // Data Alignment RIGHT

	ADC1->SMPR2 |= (7<<0) | (7<<3) | (7<<6);  // Sampling time of 239.5 cycles for channel 0, channel 1 and channel 2

	ADC1->SQR1 |= (2<<20);   // SQR1_L =2 for 3 conversions

	// Enable DMA for ADC
	ADC1->CR2 |= (1<<8);

	// Channel Sequence
	ADC1->SQR3 |= (0<<0);
	ADC1->SQR3 |= (1<<5);
	ADC1->SQR3 |= (2<<10);
}


void enableADC (void)
{
	ADC1->CR2 |= 1<<0;   // ADON =1 enable ADC1

	uint32_t delay = 10000;
	while (delay--);
}

void startADC(void)
{
	ADC1->SR = 0;                      // Clear Status register
	ADC1->CR2 |= (1<<20);              // Conversion on external event enabled
	ADC1->CR2 |= 1<<22;                // Start conversion
}


void configDMA ()
{
		/*Configure DMA*/

		/*Enable Clock access to DMA1*/
		RCC->AHBENR|=RCC_AHBENR_DMA1EN;

		/*DMA1_Channel1 is for ADC1*/

		DMA1_Channel1->CCR = 0x000005A0;

		DMA1_Channel1->CNDTR=3;

		DMA1_Channel1->CPAR=(uint32_t)(&ADC1->DR);

		DMA1_Channel1->CMAR=(uint32_t)(adc_data);

		DMA1_Channel1->CCR|=DMA_CCR_EN;
}
