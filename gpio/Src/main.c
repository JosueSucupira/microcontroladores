#include <stdint.h>

#define SET_A0 0x00000001
#define SET_A1 0x00000002
#define SET_A2 0x00000004

int main(void)
{
    // Registradores RCC_APB2ENR, GPIOA_CRL, GPIOB_CRL, GPIOB_CRH, GPIOA_IDR e GPIOB_ODR
    volatile uint32_t *pRCC_APB2ENR = (uint32_t *)0x40021018;
    volatile uint32_t *pGPIOA_CRL = (uint32_t *)0x40010800;
    volatile uint32_t *pGPIOB_CRL = (uint32_t *)0x40010C00;
    volatile uint32_t *pGPIOB_CRH = (uint32_t *)0x40010C04;
    volatile uint32_t *pGPIOB_ODR = (uint32_t *)0x40010C0C;
    volatile uint32_t *pGPIOA_IDR = (uint32_t *)0x40010808;

    // Habilita o clock da Porta A e B
    *pRCC_APB2ENR |= 0x0C;

    // Reseta os bits
    *pGPIOA_CRL &= 0xFFFFF000;
    *pGPIOB_CRL &= 0xFFFFFF00;
    *pGPIOB_CRH &= 0xFFFFF0FF;

    // Seta os bits para Input A0, A1 e A2
    *pGPIOA_CRL |= 0x00000888;

    // Seta os bits para output B0 e B1
    *pGPIOB_CRL |= 0x00000022;

    // Seta o B10 para output
    *pGPIOB_CRH |= 0x00000200;

    // Seta os pinos 0, 1 e 10 da Porta B (apaga o LED)
    *pGPIOB_ODR |= 0x00000403;

    for(;;)
    {
        // Verifica se o botão no pino A0 está pressionado
        if ((*pGPIOA_IDR & SET_A0) != 0)
        {
            *pGPIOB_ODR |= 0x00000001; // Acende o LED no pino B0
        }
        else
        {
            *pGPIOB_ODR &= ~0x00000001; // Apaga o LED no pino B0
        }

        // Verifica se o botão no pino A1 está pressionado
        if ((*pGPIOA_IDR & SET_A1) != 0)
        {
            *pGPIOB_ODR |= 0x00000002; // Acende o LED no pino B1
        }
        else
        {
            *pGPIOB_ODR &= ~0x00000002; // Apaga o LED no pino B1
        }

        // Verifica se o botão no pino A2 está pressionado
        if ((*pGPIOA_IDR & SET_A2) != 0)
        {
            *pGPIOB_ODR |= 0x00000400; // Acende o LED no pino B10
        }
        else
        {
            *pGPIOB_ODR &= ~0x00000400; // Apaga o LED no pino B10
        }
    }
}
