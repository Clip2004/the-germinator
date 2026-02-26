/*
 * PINS.h
 *
 *  Created on: May 14, 2025
 *      Author: juanj
 */

#ifndef INC_PINS_H_
#define INC_PINS_H_

#define TEST_LED_PORT GPIOB
#define SOAK_LED_PORT GPIOA
#define REFLOW_LED_PORT GPIOB
#define COOLING_LED_PORT GPIOB

#define TEST_LED_PIN GPIO_PIN_1
#define SOAK_LED_PIN GPIO_PIN_3
#define REFLOW_LED_PIN GPIO_PIN_10
#define COOLING_LED_PIN GPIO_PIN_12

#define RA_PWM_TIMER &htim2
#define RA_PWM_CHANNEL TIM_CHANNEL_1

#define RB_PWM_TIMER &htim2
#define RB_PWM_CHANNEL TIM_CHANNEL_4

#define HEATER_PIN GPIO_PIN_1  // PA1: Resistencia AC
#define HEATER_PORT GPIOA
#define FAN_PIN    GPIO_PIN_2  // PA2: Ventilador AC
#define FAN_PORT   GPIOA


#define CEn_Pin GPIO_PIN_3
#define CEn_GPIO_Port GPIOF



#endif /* INC_PINS_H_ */
