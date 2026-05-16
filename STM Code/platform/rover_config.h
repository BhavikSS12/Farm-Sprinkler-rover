#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

#include <stdint.h>

#define F_CPU             72000000UL
#define APB1_TIMER_CLK    72000000UL
#define USART1_BRR_VALUE  625U

#define PWM_PSC           281U
#define PWM_ARR           255U

#define DEBOUNCE_US       1500U

#define PERIPH_BASE       0x40000000UL
#define APB1PERIPH_BASE   PERIPH_BASE
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE    (PERIPH_BASE + 0x00020000UL)

#define TIM2_BASE         (APB1PERIPH_BASE + 0x00000000UL)
#define TIM3_BASE         (APB1PERIPH_BASE + 0x00000400UL)
#define TIM4_BASE         (APB1PERIPH_BASE + 0x00000800UL)
#define RCC_BASE          (AHBPERIPH_BASE  + 0x00001000UL)
#define FLASH_R_BASE      (AHBPERIPH_BASE  + 0x00002000UL)
#define AFIO_BASE         (APB2PERIPH_BASE + 0x00000000UL)
#define EXTI_BASE         (APB2PERIPH_BASE + 0x00000400UL)
#define GPIOA_BASE        (APB2PERIPH_BASE + 0x00000800UL)
#define GPIOB_BASE        (APB2PERIPH_BASE + 0x00000C00UL)
#define USART1_BASE       (APB2PERIPH_BASE + 0x00003800UL)

#define SYST_CSR          (*(volatile uint32_t *)0xE000E010UL)
#define SYST_RVR          (*(volatile uint32_t *)0xE000E014UL)
#define SYST_CVR          (*(volatile uint32_t *)0xE000E018UL)
#define NVIC_ISER0        (*(volatile uint32_t *)0xE000E100UL)
#define DWT_CTRL          (*(volatile uint32_t *)0xE0001000UL)
#define DWT_CYCCNT        (*(volatile uint32_t *)0xE0001004UL)
#define DEMCR             (*(volatile uint32_t *)0xE000EDFCUL)

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t AR;
    uint32_t RESERVED;
    volatile uint32_t OBR;
    volatile uint32_t WRPR;
} FLASH_TypeDef;

typedef struct {
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4];
    uint32_t RESERVED0;
    volatile uint32_t MAPR2;
} AFIO_TypeDef;

typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;

typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    uint32_t RESERVED0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    uint32_t RESERVED1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

#define RCC      ((RCC_TypeDef   *)RCC_BASE)
#define FLASH    ((FLASH_TypeDef *)FLASH_R_BASE)
#define AFIO     ((AFIO_TypeDef  *)AFIO_BASE)
#define EXTI     ((EXTI_TypeDef  *)EXTI_BASE)
#define GPIOA    ((GPIO_TypeDef  *)GPIOA_BASE)
#define GPIOB    ((GPIO_TypeDef  *)GPIOB_BASE)
#define TIM2     ((TIM_TypeDef   *)TIM2_BASE)
#define TIM3     ((TIM_TypeDef   *)TIM3_BASE)
#define TIM4     ((TIM_TypeDef   *)TIM4_BASE)
#define USART1   ((USART_TypeDef *)USART1_BASE)

#define LED_PIN_BIT        2U

#define R_EN_A_BIT         0U
#define L_EN_A_BIT         1U
#define RPWM_A_BIT         4U
#define LPWM_A_BIT         5U

#define RPWM_B_BIT         6U
#define LPWM_B_BIT         7U
#define R_EN_B_BIT         8U
#define L_EN_B_BIT         9U

#define ENCODER_A_BIT      0U
#define ENCODER_B_BIT      1U

#define PUMP_IN1_BIT       2U
#define PUMP_IN2_BIT       3U
#define PUMP_ENA_BIT       3U

#define RCC_CR_HSEON            (1U << 16)
#define RCC_CR_HSERDY           (1U << 17)
#define RCC_CR_PLLON            (1U << 24)
#define RCC_CR_PLLRDY           (1U << 25)

#define RCC_CFGR_SW_PLL         (0x2U)
#define RCC_CFGR_SWS_MASK       (0xCU)
#define RCC_CFGR_SWS_PLL        (0x8U)
#define RCC_CFGR_HPRE_DIV1      (0x0U << 4)
#define RCC_CFGR_PPRE1_DIV2     (0x4U << 8)
#define RCC_CFGR_PPRE2_DIV1     (0x0U << 11)
#define RCC_CFGR_PLLSRC_HSE     (1U << 16)
#define RCC_CFGR_PLLMULL9       (0x7U << 18)

#define RCC_APB2ENR_AFIOEN      (1U << 0)
#define RCC_APB2ENR_IOPAEN      (1U << 2)
#define RCC_APB2ENR_IOPBEN      (1U << 3)
#define RCC_APB2ENR_USART1EN    (1U << 14)

#define RCC_APB1ENR_TIM2EN      (1U << 0)
#define RCC_APB1ENR_TIM3EN      (1U << 1)
#define RCC_APB1ENR_TIM4EN      (1U << 2)

#define FLASH_ACR_PRFTBE        (1U << 4)
#define FLASH_ACR_LATENCY_2     (0x2U)

#define TIM_CR1_CEN             (1U << 0)
#define TIM_CR1_ARPE            (1U << 7)
#define TIM_CCMR1_OC1PE         (1U << 3)
#define TIM_CCMR1_OC1M_PWM1     (0x6U << 4)
#define TIM_CCMR1_OC2PE         (1U << 11)
#define TIM_CCMR1_OC2M_PWM1     (0x6U << 12)
#define TIM_CCER_CC1E           (1U << 0)
#define TIM_CCER_CC2E           (1U << 4)
#define TIM_EGR_UG              (1U << 0)

#define USART_SR_RXNE           (1U << 5)
#define USART_SR_TXE            (1U << 7)
#define USART_CR1_UE            (1U << 13)
#define USART_CR1_TE            (1U << 3)
#define USART_CR1_RE            (1U << 2)

#define DEMCR_TRCENA            (1U << 24)
#define DWT_CTRL_CYCCNTENA      (1U << 0)

#endif