#include "../platform/rover_config.h"
#include "board.h"

static volatile uint32_t ms_ticks = 0;

static void pin_set_mode(GPIO_TypeDef *gpio, uint8_t pin, uint8_t mode4)
{
    volatile uint32_t *cr = (pin < 8u) ? &gpio->CRL : &gpio->CRH;
    uint8_t shift = (pin & 7u) << 2u;
    *cr = (*cr & ~(0xFu << shift)) | ((uint32_t)mode4 << shift);
}

static void clock_init(void)
{
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    RCC->CFGR = RCC_CFGR_PLLSRC_HSE |
                RCC_CFGR_PLLMULL9   |
                RCC_CFGR_HPRE_DIV1  |
                RCC_CFGR_PPRE1_DIV2 |
                RCC_CFGR_PPRE2_DIV1;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_MASK) != RCC_CFGR_SWS_PLL);
}

static void systick_init(void)
{
    SYST_RVR = 72000UL - 1UL;
    SYST_CVR = 0;
    SYST_CSR = 0x7U;
}

static void dwt_init(void)
{
    DEMCR |= DEMCR_TRCENA;
    DWT_CYCCNT = 0;
    DWT_CTRL |= DWT_CTRL_CYCCNTENA;
}

static void gpio_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;

    uint32_t mapr = AFIO->MAPR;
    mapr &= ~((0x7U << 24) | (0x3U << 8) | (0x3U << 10));
    mapr |=  (0x2U << 24) | (0x1U << 8) | (0x2U << 10);
    AFIO->MAPR = mapr;

    pin_set_mode(GPIOB, LED_PIN_BIT, 0x2u);
    pin_set_mode(GPIOB, R_EN_A_BIT, 0x2u);
    pin_set_mode(GPIOB, L_EN_A_BIT, 0x2u);
    pin_set_mode(GPIOB, R_EN_B_BIT, 0x2u);
    pin_set_mode(GPIOB, L_EN_B_BIT, 0x2u);

    pin_set_mode(GPIOB, PUMP_ENA_BIT, 0xAu);
    pin_set_mode(GPIOB, RPWM_A_BIT, 0xAu);
    pin_set_mode(GPIOB, LPWM_A_BIT, 0xAu);
    pin_set_mode(GPIOB, RPWM_B_BIT, 0xAu);
    pin_set_mode(GPIOB, LPWM_B_BIT, 0xAu);

    pin_set_mode(GPIOA, PUMP_IN1_BIT, 0x2u);
    pin_set_mode(GPIOA, PUMP_IN2_BIT, 0x2u);

    pin_set_mode(GPIOA, ENCODER_A_BIT, 0x8u);
    pin_set_mode(GPIOA, ENCODER_B_BIT, 0x8u);
    GPIOA->BSRR = (1U << ENCODER_A_BIT) | (1U << ENCODER_B_BIT);

    pin_set_mode(GPIOA, 9U, 0xAu);
    pin_set_mode(GPIOA, 10U, 0x4u);

    GPIOB->BSRR = (1U << LED_PIN_BIT);
    GPIOB->BRR  = (1U << R_EN_A_BIT) | (1U << L_EN_A_BIT) |
                  (1U << R_EN_B_BIT) | (1U << L_EN_B_BIT);
    GPIOA->BRR  = (1U << PUMP_IN1_BIT) | (1U << PUMP_IN2_BIT);
}

void SysTick_Handler(void)
{
    ms_ticks++;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = ms_ticks;
    while ((ms_ticks - start) < ms);
}

uint32_t micros(void)
{
    return DWT_CYCCNT / (F_CPU / 1000000UL);
}

void uart1_putc(char c)
{
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = (uint8_t)c;
}

void uart1_puts(const char *s)
{
    while (*s) uart1_putc(*s++);
}

void led_on(void)  { GPIOB->BRR  = (1U << LED_PIN_BIT); }
void led_off(void) { GPIOB->BSRR = (1U << LED_PIN_BIT); }

void board_init(void)
{
    clock_init();
    systick_init();
    dwt_init();
    gpio_init();
}