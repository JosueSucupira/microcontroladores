#include "stm32f1xx.h"
#include <stdint.h>

// Define colors
#define RED     (1 << 0)
#define GREEN   (1 << 1)
#define BLUE    (1 << 10)

volatile uint8_t color_index = 0;
volatile uint8_t blink_count = 0;

const uint32_t colors[] = {
    RED,         // Red
    GREEN,       // Green
    BLUE,        // Blue
    RED | GREEN, // Yellow
    GREEN | BLUE,// Cyan
    RED | BLUE,  // Purple
    RED | GREEN | BLUE // White
};


void timer2Config(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 8000-1; //8MHz  1000Hz
	TIM2->ARR = 500-1;

	TIM2->DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->CR1 |= TIM_CR1_CEN;
}


void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF)
	{

		TIM2->SR &=~TIM_SR_UIF; // limpa a flag

		GPIOB->ODR ^= colors[color_index]; // muda as cores

		blink_count++;
		if(blink_count >= 4){

			blink_count = 0;

			GPIOB->ODR &= ~(RED | GREEN | BLUE);
			color_index++;

		}
		if(color_index >= 7){
			color_index = 0;
		}
	}
}

int main(void){

	// Ativa o clock para GPIOB
	RCC->APB2ENR |= (1 << 3);
	// Configura PB0, PB1 e PB10 como saída
	GPIOB->CRL &= 0xFFFFFF00;
	GPIOB->CRH &= 0xFFFFF0FF;
	GPIOB->CRL |= 0x00000022;
	GPIOB->CRH |= 0x00000200;

	//apagado
	GPIOB->ODR &= ~(RED | GREEN | BLUE);

	timer2Config();

    while (1){
    }

}
