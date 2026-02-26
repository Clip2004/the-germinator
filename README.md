# 🌱 The Germinator — Smart Seed Germination System

> **Español:** The Germinator es una germinadora de semillas inteligente desarrollada como proyecto final de la materia de Internet of Things en la Escuela de Ingeniería de Antioquia (EIA). El sistema monitorea y controla temperatura, humedad y luz para optimizar la tasa de germinación en distintos tipos de semillas.

---

## 📖 Overview

The Germinator is an IoT-based smart germination chamber designed to maximize seed germination rates by continuously monitoring and controlling key environmental variables:

| Variable | Sensor | Actuator |
|---|---|---|
| 🌡️ Temperature | DHT11 | Fan (ventilation control) |
| 💧 Humidity | DHT11 | — |
| 💡 Light | LDR | Lamp (light control) |

The system collects real-time sensor data, transmits it over WiFi via MQTT, stores it in a SQL database, and visualizes it through a live Grafana dashboard — enabling continuous and remote monitoring of the germination environment.

---

## 🏗️ System Architecture

```
┌─────────────────────────────────┐
│        Germination Chamber      │
│                                 │
│  [DHT11] ──┐                    │
│  [LDR]   ──┤── STM32 + WiFi ───────────► MQTT Broker
│            │       │            │
│          [Fan]  [Lamp]          │
└─────────────────────────────────┘
                                        │
                              ┌─────────▼──────────┐
                              │   MQTT Subscriber   │
                              │     (Python)        │
                              └─────────┬──────────┘
                                        │
                              ┌─────────▼──────────┐
                              │    SQL Database     │
                              └─────────┬──────────┘
                                        │
                              ┌─────────▼──────────┐
                              │  Grafana Dashboard  │
                              │  (Live Monitoring)  │
                              └────────────────────┘
```

---

## 🔧 Hardware

### Electronics
- **Microcontroller:** STM32 + WiFi module
- **Sensors:** DHT11 (temperature & humidity), LDR (ambient light)
- **Actuators:** DC fan, lamp
- **Custom PCB:** Designed from scratch following electronic design rules

### PCB Manufacturing Process
1. Schematic design and PCB layout in KiCad
2. Component selection and Bill of Materials (BOM)
3. Assembly using **pick and place** machine
4. Solder reflow in **reflow oven**

---

## 🌐 Software & IoT Stack

| Layer | Technology |
|---|---|
| Firmware | STM32 (C/C++) |
| Communication | WiFi + MQTT |
| Data pipeline | Python (MQTT subscriber) |
| Database | SQL |
| Visualization | Grafana |

---

## 📁 Repository Structure

```
the-germinator/
├── backend/
│   ├── mqtt/
│   │   ├── index.js
│   │   └── package.json
│   └── sql/
│       └── germinator.sql
├── docs/
├── firmware/
│   └── stm32/
│       ├── Inc/
│       │   ├── credentials.example.h
│       │   ├── PINS.h
│       │   ├── dht11_nonblocking.h
│       │   ├── main.h
│       │   ├── stm32c0xx_hal_conf.h
│       │   └── stm32c0xx_it.h
│       └── Src/
│           ├── FC41/
│           ├── STATES/
│           ├── UART/
│           ├── publoop/
│           ├── timer/
│           ├── main.c
│           └── dht11_nonblocking.c
├── grafana/
│   └── GerminatorDashboards.json
├── hardware/
│   ├── bom/
│   ├── datasheets/
│   └── pcb/
│       ├── PCB.pdf
│       ├── Schematics.pdf
│       └── pcb_print.pdf
├── media/
│   ├── photos/
│   │   ├── grafana_dashboard.png
│   │   └── mounted_pcb.jpeg
│   └── videos/
│       └── the_germinator.mov
└── tools/
    └── simulator/
        └── mqtt_publisher.js
```

---

## 🚀 Getting Started

### Prerequisites
- Docker & docker-compose
- STM32 development environment (STM32CubeIDE or PlatformIO)
- MQTT broker (Mosquitto, included in docker-compose)

### 1. Clone the repository
```bash
git clone https://github.com/your-username/the-germinator.git
cd the-germinator
```

### 2. Configure environment variables
```bash
cp .env.example .env
# Edit .env with your WiFi credentials and MQTT broker IP
```

### 3. Launch backend services
```bash
cd backend
docker-compose up -d
```

### 4. Flash the firmware
Open the `firmware/stm32/` project in STM32CubeIDE, update `config.h` with your network settings, and flash to the board.

### 5. Open Grafana
Navigate to `http://localhost:3000` and import `grafana/dashboard.json`.

---

## 📸 Gallery

> *Photos of the assembled PCB, chamber, and Grafana dashboard coming soon.*

---

## 👥 Team

| Name | Role |
|---|---|
| **[Felipe Mercado Mercado]** | Hardware design, firmware, IoT integration |
| **Samuel Muñoz Arias** | Hardware design, firmware, IoT integration |

**Academic advisor:** José Valentín Antonio Restrepo Laverde
**Institution:** Escuela de Ingeniería de Antioquia (EIA)
**Course:** Internet of Things
**Year:** 2024

---

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

*Made with 🌱 at EIA — Escuela de Ingeniería de Antioquia*
