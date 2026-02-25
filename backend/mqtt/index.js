// ====================================================
// backend/mqtt/index.js
// Inserts MQTT data into MySQL - sensor_log table
// ====================================================

require('dotenv').config();
const mqtt = require('mqtt');
const mysql = require('mysql');

// -------------------------------------
// 🔌 MQTT BROKER CONFIGURATION
// -------------------------------------
const MQTT_BROKER = process.env.MQTT_BROKER;
const MQTT_TOPIC  = process.env.MQTT_TOPIC;

if (!MQTT_BROKER || !MQTT_TOPIC) {
  console.error('❌ Missing MQTT_BROKER or MQTT_TOPIC environment variables. Check your .env file');
  process.exit(1);
}

// -------------------------------------
// 🗄️ MYSQL CONFIGURATION
// -------------------------------------
const sqlcon = mysql.createConnection({
  host:     process.env.DB_HOST,
  user:     process.env.DB_USER,
  password: process.env.DB_PASSWORD,
  database: process.env.DB_NAME,
  port:     parseInt(process.env.DB_PORT) || 3306
});

// -------------------------------------
// ⚙️ MYSQL CONNECTION
// -------------------------------------
function connectMySQL() {
  sqlcon.connect((err) => {
    if (err) {
      console.error('❌ Error connecting to MySQL:', err.message);
      setTimeout(connectMySQL, 5000);
    } else {
      console.log(`✅ Connected to MySQL (${process.env.DB_NAME})`);
    }
  });

  sqlcon.on('error', (err) => {
    console.error('⚠️ MySQL connection error:', err);
    if (err.code === 'PROTOCOL_CONNECTION_LOST') {
      connectMySQL();
    } else {
      throw err;
    }
  });
}

// -------------------------------------
// 📡 MQTT CONNECTION
// -------------------------------------
const client = mqtt.connect(MQTT_BROKER);

client.on('connect', () => {
  console.log(`✅ Connected to MQTT broker: ${MQTT_BROKER}`);
  client.subscribe(MQTT_TOPIC, (err) => {
    if (err) {
      console.error('❌ Error subscribing to topic:', err);
    } else {
      console.log(`📡 Subscribed to topic: ${MQTT_TOPIC}`);
    }
  });
});

client.on('error', (err) => {
  console.error('⚠️ MQTT connection error:', err);
});

client.on('reconnect', () => {
  console.log('🔄 Reconnecting to MQTT...');
});

// -------------------------------------
// 🧠 MQTT MESSAGE PROCESSING
// -------------------------------------
client.on('message', (topic, message) => {
  console.log(`📥 Message received on ${topic}: ${message.toString()}`);

  try {
    const data = JSON.parse(message.toString());
    const { batch_id, temperature_celsius, humidity_percent, light_intensity_percentage } = data;

    if (
      batch_id === undefined ||
      temperature_celsius === undefined ||
      humidity_percent === undefined ||
      light_intensity_percentage === undefined
    ) {
      console.warn('⚠️ Incomplete or malformed message, ignoring.');
      return;
    }

    const sql = `
      INSERT INTO sensor_log (
        batch_id,
        temperature_celsius,
        humidity_percent,
        light_intensity_percentage,
        timestamp
      ) VALUES (?, ?, ?, ?, NOW());
    `;

    sqlcon.query(
      sql,
      [batch_id, temperature_celsius, humidity_percent, light_intensity_percentage],
      (err, result) => {
        if (err) {
          console.error('❌ Error inserting into MySQL:', err);
        } else {
          console.log('✅ Record inserted into sensor_log (ID:', result.insertId, ')');
        }
      }
    );

  } catch (err) {
    console.error('❌ Error processing MQTT message:', err.message);
  }
});

// -------------------------------------
// 🚀 START MYSQL CONNECTION
// -------------------------------------
connectMySQL();