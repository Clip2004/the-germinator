#include "dht11_nonblocking.h"
#include <string.h>

/* --- Variables Privadas --- */
static TIM_HandleTypeDef *dht_timer;
static GPIO_TypeDef *dht_port;
static uint16_t dht_pin;

static volatile DHT_Status_t dht_status = DHT_STATUS_IDLE;

typedef enum {
    SM_IDLE,
    SM_START_PULL_LOW,
    SM_START_WAIT_HIGH,
    SM_RESPONSE_WAIT_LOW,
    SM_RESPONSE_WAIT_HIGH,
    SM_DATA_WAIT_LOW,
    SM_DATA_WAIT_HIGH,
    SM_DATA_READ_PULSE,
    SM_PROCESS_DATA,
    SM_ERROR
} DHT_SM_State_t;

static DHT_SM_State_t dht_sm_state = SM_IDLE;

static uint16_t dht_last_event_time_us;
static uint8_t  dht_data_buffer[5];
static uint8_t  dht_bit_index;

static float dht_temperature = 0.0f;
static float dht_humidity = 0.0f;
static bool  dht_new_data_available = false;

/* --- Funciones Auxiliares --- */

static uint16_t get_micros(void) {
    return (uint16_t)__HAL_TIM_GET_COUNTER(dht_timer);
}

/* * EN MODO OPEN DRAIN:
 * - Escribir 0 (RESET): El pin se conecta a Tierra (GND).
 * - Escribir 1 (SET): El pin queda "flotando" y la resistencia pull-up lo sube a 3.3V.
 * En este estado (1), podemos LEER lo que envía el sensor.
 */

void DHT_Init(TIM_HandleTypeDef *htim_us, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    dht_timer = htim_us;
    dht_port = GPIOx;
    dht_pin = GPIO_Pin;

    HAL_TIM_Base_Start(dht_timer);

    // Asegurar estado inicial alto (liberado)
    HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET);

    dht_sm_state = SM_IDLE;
    dht_status = DHT_STATUS_IDLE;
}

void DHT_Start_Read(void) {
    if (dht_sm_state == SM_IDLE) {
        dht_sm_state = SM_START_PULL_LOW;
        dht_status = DHT_STATUS_BUSY;
        dht_new_data_available = false;
    }
}

bool DHT_GetData(float *temp, float *humid) {
    if (dht_new_data_available) {
        *temp = dht_temperature;
        *humid = dht_humidity;
        dht_new_data_available = false;
        return true;
    }
    return false;
}

DHT_Status_t DHT_Get_Status(void) {
    return dht_status;
}

static void process_data_buffer(void) {
    // Verificación simple de checksum
    uint8_t sum = dht_data_buffer[0] + dht_data_buffer[1] + dht_data_buffer[2] + dht_data_buffer[3];

    if (dht_data_buffer[4] == sum) {
        // Conversión para DHT11 (solo parte entera suele ser válida)
        dht_humidity = (float)dht_data_buffer[0];
        // Si tu sensor soporta decimales: + (float)dht_data_buffer[1] / 10.0f;

        dht_temperature = (float)dht_data_buffer[2];
        // Si tu sensor soporta decimales: + (float)dht_data_buffer[3] / 10.0f;

        dht_new_data_available = true;
        dht_status = DHT_STATUS_OK;
    } else {
        dht_status = DHT_STATUS_ERROR; // Checksum error
    }
    dht_sm_state = SM_IDLE;
}

void DHT_Process(void) {
    uint16_t current_time_us = get_micros();
    uint16_t elapsed_time_us = current_time_us - dht_last_event_time_us;

    switch(dht_sm_state) {
        case SM_IDLE:
            break;

        case SM_START_PULL_LOW:
            // Bajamos la línea (Start Signal)
            HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_RESET);
            dht_last_event_time_us = current_time_us;
            dht_sm_state = SM_START_WAIT_HIGH;
            break;

        case SM_START_WAIT_HIGH:
            // Esperamos 19ms (19000 us)
            if (elapsed_time_us >= 19000) {
                // Soltamos la línea (Se va a High por pull-up)
                HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET);
                dht_last_event_time_us = current_time_us;
                dht_sm_state = SM_RESPONSE_WAIT_LOW;
            }
            break;

        case SM_RESPONSE_WAIT_LOW:
            // Esperamos que el sensor baje la línea (Response)
            if (HAL_GPIO_ReadPin(dht_port, dht_pin) == GPIO_PIN_RESET) {
                dht_last_event_time_us = current_time_us;
                dht_sm_state = SM_RESPONSE_WAIT_HIGH;
            } else if (elapsed_time_us > 100) {
                dht_status = DHT_STATUS_ERROR; // Timeout esperando respuesta
                dht_sm_state = SM_IDLE;
                HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET); // Liberar línea
            }
            break;

        case SM_RESPONSE_WAIT_HIGH:
            // El sensor mantiene bajo ~80us, luego sube
            if (HAL_GPIO_ReadPin(dht_port, dht_pin) == GPIO_PIN_SET) {
                dht_last_event_time_us = current_time_us;
                dht_sm_state = SM_DATA_WAIT_LOW;
            } else if (elapsed_time_us > 100) {
                dht_status = DHT_STATUS_ERROR;
                dht_sm_state = SM_IDLE;
                 HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET);
            }
            break;

        case SM_DATA_WAIT_LOW:
            // El sensor mantiene alto ~80us, luego baja para empezar datos
            if (HAL_GPIO_ReadPin(dht_port, dht_pin) == GPIO_PIN_RESET) {
                dht_last_event_time_us = current_time_us;
                dht_sm_state = SM_DATA_WAIT_HIGH;
                dht_bit_index = 0;
                memset(dht_data_buffer, 0, 5);
            } else if (elapsed_time_us > 100) {
                dht_status = DHT_STATUS_ERROR;
                dht_sm_state = SM_IDLE;
                 HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET);
            }
            break;

        case SM_DATA_WAIT_HIGH:
            // Inicio del bit: espera que termine el tiempo en bajo (~50us)
            if (HAL_GPIO_ReadPin(dht_port, dht_pin) == GPIO_PIN_SET) {
                dht_last_event_time_us = current_time_us; // Marca inicio del pulso HIGH
                dht_sm_state = SM_DATA_READ_PULSE;
            } else if (elapsed_time_us > 80) {
                dht_status = DHT_STATUS_ERROR;
                dht_sm_state = SM_IDLE;
                 HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET);
            }
            break;

        case SM_DATA_READ_PULSE:
            // Midiendo duración del pulso HIGH
            if (HAL_GPIO_ReadPin(dht_port, dht_pin) == GPIO_PIN_RESET) {
                uint16_t pulse_len = elapsed_time_us;

                // '0' es ~26-28us, '1' es ~70us. Punto de corte: 40-50us
                if (pulse_len > 45) {
                    dht_data_buffer[dht_bit_index / 8] |= (1 << (7 - (dht_bit_index % 8)));
                }

                dht_bit_index++;
                dht_last_event_time_us = current_time_us; // Marca inicio del siguiente low

                if (dht_bit_index >= 40) {
                    dht_sm_state = SM_PROCESS_DATA;
                } else {
                    dht_sm_state = SM_DATA_WAIT_HIGH;
                }
            } else if (elapsed_time_us > 100) {
                dht_status = DHT_STATUS_ERROR; // Pulso demasiado largo
                dht_sm_state = SM_IDLE;
                 HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET);
            }
            break;

        case SM_PROCESS_DATA:
            process_data_buffer();
            HAL_GPIO_WritePin(dht_port, dht_pin, GPIO_PIN_SET); // Asegurar línea libre
            break;

        default:
            dht_sm_state = SM_IDLE;
            break;
    }
}
