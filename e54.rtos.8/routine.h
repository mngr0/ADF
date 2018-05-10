/*
* routine.h
*
* Created: 03/05/2018 17:31:32
*  Author: COLLAUDI E SVILUPPO
*/


#ifndef ROUTINE_H_
#define ROUTINE_H_

#include <atmel_start.h>
#include "rtos_start.h"

#define TASK_EXAMPLE_STACK_SIZE (128 / sizeof(portSTACK_TYPE))
#define TASK_EXAMPLE_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

#define len(p) formats[p].len
#define times(p) formats[p].times
#define overtime(p) formats[p].overtime
#define undertime(p) formats[p].undertime
#define dot_pause(p) 0// formats[p].dot_pause
#define dot_len(p) 0// formats[p].dot_len

extern TaskHandle_t xRoutineTask0;
extern TaskHandle_t xRoutineTask1;

void static open_over(uint8_t index){
	if (index){
		gpio_set_pin_level(PB04,1);
	}
	else{
		gpio_set_pin_level(PB05,1);
	}
}

void static open_normal(uint8_t index) {
	//dot mode, still pwm
	if (index){
		gpio_set_pin_level(PB15,1);
	}
	else{
		gpio_set_pin_level(PB09,1);
	}
}

void static open_under(uint8_t index) {
	if (index){
		PWM_1_init();
		pwm_set_parameters(&PWM_1,  1000,  500);
		pwm_enable(&PWM_1);
	}
	else{
		PWM_0_init();
		pwm_set_parameters(&PWM_0,  1000,  500);
		pwm_enable(&PWM_0);
	}
}

void static close_over(uint8_t index){
	if (index){
		gpio_set_pin_level(PB04,0);
	}
	else{
		gpio_set_pin_level(PB05,0);
	}
}


void static close_normal(uint8_t index){
	if (index){
		gpio_set_pin_level(PB15,0);
	}
	else{
		gpio_set_pin_level(PB09,0);
	}
}

void static close_under(uint8_t index){
	if (index){
		pwm_disable(&PWM_1);
		gpio_set_pin_direction(PB15, GPIO_DIRECTION_OUT);
		gpio_set_pin_function(PB15, GPIO_PIN_FUNCTION_OFF);
		gpio_set_pin_level(PB15,0);
	}
	else{
		pwm_disable(&PWM_0);
		gpio_set_pin_direction(PB09, GPIO_DIRECTION_OUT);
		gpio_set_pin_function(PB09, GPIO_PIN_FUNCTION_OFF);
		gpio_set_pin_level(PB09,0);
	}
}

void start_routine();

#endif /* ROUTINE_H_ */