# Solarco2

Dispositivo IoT para medición de Dióxido de Carbono basado en Wemos D1 Mini y sensor SCD30 (Sensirion), vinculado a Thingsboard con alarmas vía Telegram.

Desarrollado en equipo junto a Luciano Lamaita.

## Características
- Wemos D1 Mini
- Sensor SCD30 (Sensirión)
- Conexión a Thingsboard (MQTT)
- Conexión vía WifiManager
- Actualizaciones OTA en la misma red
## Conexión

|SCD30 | ESP8266 |
|------|---------|
| GND  |   GND   |
| VCC  |   3V3   |
| SCL  | SCL (D1)|
| SDA  | SDA (D2)|

