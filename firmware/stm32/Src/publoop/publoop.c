/*
 * publoop.c
 *
 *  Created on: Oct 30, 2024
 *      Author: Laboratorio UPB
 */

#include "../FC41/FC41.h"
#include "../UART/uart.h"
#include "../timer/timer.h"
#include "../STATES/states.h"
#include <string.h>
#include <stdio.h>
#include "main.h"

extern int since_start_time;

extern float temperature;
extern float humidity;

extern int Running;
extern int dht_debug_status;
int cont = 0;

unsigned int pl_tick;
unsigned int p2_tick;
int pl_timeout=0;
int p2_timeout=0;

void pub_loop(void) {
	if (get_tick()!=pl_tick) {
		pl_tick = get_tick();
		pl_timeout++;
		if (pl_timeout>=240) {
			Running = states.mqtt.payload[0];
			publishJSON("germinator/insert/sensor_log","batch_id","temperature_celsius","humidity_percent","light_intensity_percentage", 1, temperature, 80, 70);
			//publish("vatin", "hola");
//			publishJSON_2("germinator/insert/germination_profiles","plant_specie","profile_name","temp_min_celsius","temp_max_celsius", "a", "b", "c", "d");

			pl_timeout=0;
			cont++;
		}
		return;
	}
}

