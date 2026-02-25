// ====================================================
// tools/simulator/mqtt_publisher.js
// Microcontroller emulator publishing MQTT data
// For development and local testing only
// ====================================================

require('dotenv').config({ path: '../../.env' });
const mqtt = require('mqtt');

// -----------------------------
// ⚙️ CONFIGURATION
// -----------------------------
const MQTT_BROKER         = process.env.MQTT_BROKER         || 'mqtt://localhost';
const MQTT_TOPIC          = process.env.MQTT_TOPIC          || 'germinator/insert/sensor_log';
const PUBLISH_INTERVAL_MS = parseInt(process.env.PUBLISH_INTERVAL_MS) || 5000;
const BATCH_ID            = parseInt(process.env.BATCH_ID)  || 1;

// -----------------------------
// 🚀 BROKER CONNECTION
// -----------------------------
const client = mqtt.connect(MQTT_BROKER);

client.on('connect', () => {
  console.log(`✅ Publisher connected to MQTT broker (${MQTT_BROKER})`);
  console.log(`📦 Simulating batch_id: ${BATCH_ID} — Interval: ${PUBLISH_INTERVAL_MS}ms`);
  startPublishing();
});

client.on('error', (err) => {
  console.error('❌ MQTT connection error:', err);
});

client.on('reconnect', () => {
  console.log('🔄 Reconnecting to MQTT...');
});

// -----------------------------
// 🧮 SIMULATED DATA GENERATION
// -----------------------------
function generateSensorData() {
  const temperature_celsius        = 20 + Math.random() * 10;        // between 20 and 30 °C
  const humidity_percent           = 60 + Math.random() * 30;        // between 60 and 90 %
  const light_intensity_percentage = Math.floor(70 + Math.random() * 30); // between 70 and 100 %

  return {
    batch_id: BATCH_ID,
    temperature_celsius: parseFloat(temperature_celsius.toFixed(2)),
    humidity_percent: parseFloat(humidity_percent.toFixed(2)),
    light_intensity_percentage
  };
}

// -----------------------------
// 📡 CONTINUOUS DATA PUBLISHING
// -----------------------------
function startPublishing() {
  setInterval(() => {
    const data    = generateSensorData();
    const message = JSON.stringify(data);

    client.publish(MQTT_TOPIC, message, { qos: 0, retain: false }, (err) => {
      if (err) {
        console.error('❌ Error publishing message:', err);
      } else {
        console.log(`📤 Published to ${MQTT_TOPIC}:`, message);
      }
    });
  }, PUBLISH_INTERVAL_MS);
}