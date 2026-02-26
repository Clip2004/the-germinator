/*
 * states.h
 *
 *  Created on: Oct 21, 2024
 *      Author: vatin
 */

#ifndef SRC_STATES_STATES_H_
#define SRC_STATES_STATES_H_

//#include "../SerialPC/serial_pc.h"
//#include "../CAN/CAN_defines.h"

struct STATES {

	struct {
		char looop;
		char SSID[32];
		char SSID_pass[64];
		char myIP[32];
		char connect;
		char disconnect;
		char scan;
		char rx_response[128];
		unsigned int response_size;
		char tx_command[256];
	}modem;
	struct {
		char IP_DNS[64];
		char PORT[5];
		char clientid[16];
		char login[16];
		char password[16];
		char connected;
		char new_data;
		char topic[20];
		int payloadSize;
		char payload[128];
		char busy;
	}mqtt;
	struct {
		char url[64];
		char msg[128];
		int msgSize;
		char response[5];
	}post;


};

extern struct STATES states;

#endif /* SRC_STATES_STATES_H_ */

