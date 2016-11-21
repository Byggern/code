#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

#define PWM_PERIOD_US 20000
#define PWM_MAX_US 2100
#define PWM_MIN_US 900
#define PWM_MID_US 1500

#define PWM_MAX PWM_MIN_MS/PWM_PERIOD_MS
#define PWM_MIN PWM_MAX_MS/PWM_PERIOD_MS

#define PWM_TOP_VAL 40000

#define PWM_DUTY_OFFSET PWM_MIN_US * PWM_TOP_VAL/PWM_PERIOD_US
#define PWM_DUTY_SCALER PWM_TOP_VAL * (PWM_MAX_US - PWM_MIN_US)/PWM_PERIOD_US/100
#define PWM_DUTY_MID PWM_MID_US * PWM_TOP_VAL / PWM_PERIOD_US

void PWM1_start(void);

void PWM_set_duty(uint8_t duty);

void PWM_set_joystick(int16_t duty);

void PWM_stop(void);

#endif /* PWM_DRIVER_H_ */