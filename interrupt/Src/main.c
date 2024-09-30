#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"


void EXTI0_IRQHandler(void){

   GPIOB->ODR ^= (1 << 0);
   EXTI->PR = (1 << 0);
  
}

void EXTI1_IRQHandler(void){

    GPIOB->ODR ^= (1 << 1);
    EXTI->PR = (1 << 1);
}

void EXTI2_IRQHandler(void){
 
    GPIOB->ODR ^= (1 << 10);
    EXTI->PR = (1 << 2);
}


int main(void){


	//GPIOA clock
	RCC->APB2ENR |= (1 << 2);
	//GPIOB clock
	RCC->APB2ENR |= (1 << 3);

	//GPIOA 0,1,2 input
	GPIOA->CRL &= 0xFFFFF000;
	GPIOA->CRL |= 0x00000888;

	//GPIOB 0,1 e 10 output
	GPIOB->CRL &= 0xFFFFFF00;
	GPIOB->CRH &= 0xFFFFF0FF;
	GPIOB->CRL |= 0x00000022;
	GPIOB->CRH |= 0x00000200;

	//apaga os leds
	GPIOB->ODR &= ~((1 << 0)|(1 << 1)|(1 << 10));

	//clock AFIO
	RCC->APB2ENR |= (1 << 0);

	//gpioA 0,1 e 2 vão gerar interrupção
	AFIO->EXTICR[0] |= (AFIO_EXTICR1_EXTI0_PA | AFIO_EXTICR1_EXTI1_PA | AFIO_EXTICR1_EXTI2_PA);

	//borda de subida
	EXTI->RTSR |= 0x00007;

	// Mascara EXTI0, EXTI1 e EXTI2
	EXTI->IMR |= 0x00007;

	//prioridades
	NVIC_SetPriority(EXTI0_IRQn,0x10);
	NVIC_SetPriority(EXTI1_IRQn,0x10);
	NVIC_SetPriority(EXTI2_IRQn,0x10);

	//habiitando as interrupções
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);

	__enable_irq();

	while (1){
	}

}

