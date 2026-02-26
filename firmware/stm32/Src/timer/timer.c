/*
 * timer.c
 *
 *  Created on: Oct 22, 2024
 *      Author: vatin
 */


#include "timer.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;

unsigned int timer=0;

unsigned int get_tick(void) {
	return timer;
}


void start_timer(void) {
	HAL_TIM_Base_Start_IT(&htim1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == htim1.Instance) {
		timer++;
	}
}
