# Solarco2
Dispositivo IoT para medición de Dióxido de Carbono basado en Wemos D1 Mini y sensor SCD30 (Sensirion), vinculado a Thingsboard con alarmas vía Telegram.
![SolarCO21](https://github.com/Alitux/solarco2/blob/b9d5e585069da2d68f7009a6f1ee12e4062443ef/imagenes/Foto%201.jpg)
![SolarCO22](https://github.com/Alitux/solarco2/blob/b9d5e585069da2d68f7009a6f1ee12e4062443ef/imagenes/Foto%202.jpg)
![SolarCO23](https://github.com/Alitux/solarco2/blob/b9d5e585069da2d68f7009a6f1ee12e4062443ef/imagenes/Foto%203.jpg)

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

