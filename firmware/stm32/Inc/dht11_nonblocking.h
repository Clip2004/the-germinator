#ifndef DHT11_NONBLOCKING_H
#define DHT11_NONBLOCKING_H

#include "main.h"
#include <stdbool.h>

typedef enum {
    DHT_STATUS_IDLE,
    DHT_STATUS_BUSY,
    DHT_STATUS_OK,
    DHT_STATUS_ERROR
} DHT_Status_t;

/**
 * @brief Inicializa el driver del DHT.
 * @param htim_us Handle del timer configurado a 1us por tick (ej: &htim3).
 * @param GPIOx Puerto del GPIO (ej: GPIOA).
 * @param GPIO_Pin Pin del GPIO (ej: GPIO_PIN_8).
 */
void DHT_Init(TIM_HandleTypeDef *htim_us, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Inicia una nueva lectura. Llamar cada 2 segundos aprox.
 */
void DHT_Start_Read(void);

/**
 * @brief Máquina de estados. LLAMAR EN EL WHILE(1) SIN DELAYS.
 */
void DHT_Process(void);

/**
 * @brief Obtiene los datos si están listos.
 * @return true si hay nuevos datos.
 */
bool DHT_GetData(float *temp, float *humid);

/**
 * @brief Para depuración.
 */
DHT_Status_t DHT_Get_Status(void);

#endif // DHT11_NONBLOCKING_H
