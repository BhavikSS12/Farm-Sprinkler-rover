#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

typedef enum{
	RPWM_A = 0,
	LPWM_A,
	RPWM_B,
	LPWM_B,
	PWM_pump_ena,
}PWM_channel_t;

void pwmInit(void);
void pwmSet(PWM_channel_t ch , uint8_t duty);

#endif