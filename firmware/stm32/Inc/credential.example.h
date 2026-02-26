#ifndef INC_CREDENTIALS_H_
#define INC_CREDENTIALS_H_

// -------------------------------------
// 📶 WiFi credentials
// -------------------------------------
#define DEFAULT_SSID        "your_wifi_network"
#define DEFAULT_PASS        "your_wifi_password"

// -------------------------------------
// 🌐 MQTT broker
// -------------------------------------
#define DEFAULT_IP          "broker.emqx.io"
#define DEFAULT_PORT        "1883"

// -------------------------------------
// 🔐 MQTT authentication (leave empty if broker has no auth)
// -------------------------------------
#define DEFAULT_USERMQTT    "germinator_client_01"
#define DEFAULT_LOGINMQTT   ""
#define DEFAULT_PASSMQTT    ""

// -------------------------------------
// 📡 MQTT topic
// -------------------------------------
#define DEFAULT_RXTOPIC     "germinator/insert/sensor_log"

#endif /* INC_CREDENTIALS_H_ */