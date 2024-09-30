#include <stdint.h>
#include "stm32f1xx.h"

// Define um tipo enum para cores
typedef enum {
    APAGADO,
    VERMELHO,
    VERDE,
    AZUL,
    AMARELO,
    CIANO,
    ROXO,
    BRANCO
} Cor;

// Variável para armazenar o estado atual das cores
Cor SysTick_TASK = APAGADO;

void SysTick_Handler(void)
{
    SysTick_TASK++;
    if (SysTick_TASK > BRANCO) {
        SysTick_TASK = APAGADO;  // Reseta a contagem de cores
    }
}

int main(void)
{
    // Ativa o clock para GPIOB
    RCC->APB2ENR |= (1 << 3);

    // Configura PB0, PB1 e PB10 como saída
    GPIOB->CRL &= 0xFFFFFF00;
    GPIOB->CRH &= 0xFFFFF0FF;
    GPIOB->CRL |= 0x00000022;
    GPIOB->CRH |= 0x00000200;

    // Apaga os LEDs
    GPIOB->ODR &= ~((1 << 0) | (1 << 1) | (1 << 10));

    uint32_t ticks = 1000000;
    if ((ticks - 1UL) <= 0xFFFFFFUL)
    {
        SysTick->LOAD  = (uint32_t)(ticks - 1UL);
        SysTick->VAL   = 0UL;  // Carrega o valor do contador SysTick
        SysTick->CTRL  = (0 << 2) |  // 0 -> SysTick_Clock = AHB/8
                         (1 << 1) |  // Habilita a IRQ e o Timer SysTick
                         (1 << 0);
    }

    while (1)
    {
        switch (SysTick_TASK) {
            case APAGADO:
                GPIOB->ODR &= ~((1 << 0) | (1 << 1) | (1 << 10));
                break;
            case VERMELHO:
                GPIOB->ODR |= (1 << 0);
                GPIOB->ODR &= ~((1 << 1) | (1 << 10));
                break;
            case VERDE:
                GPIOB->ODR |= (1 << 1);
                GPIOB->ODR &= ~((1 << 0) | (1 << 10));
                break;
            case AZUL:
                GPIOB->ODR |= (1 << 10);
                GPIOB->ODR &= ~((1 << 0) | (1 << 1));
                break;
            case AMARELO:
                GPIOB->ODR |= (1 << 0) | (1 << 1);
                GPIOB->ODR &= ~(1 << 10);
                break;
            case CIANO:
                GPIOB->ODR |= (1 << 1) | (1 << 10);
                GPIOB->ODR &= ~(1 << 0);
                break;
            case ROXO:
                GPIOB->ODR |= (1 << 0) | (1 << 10);
                GPIOB->ODR &= ~(1 << 1);
                break;
            case BRANCO:
                GPIOB->ODR |= (1 << 0) | (1 << 1) | (1 << 10);
                break;
            default:
                SysTick_TASK = APAGADO;
                break;
        }
    }
}
