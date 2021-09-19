# Project Aether

Measure and display the air quality.

This project measures the air quality with a microcontroller, the data is sent via MQTT to Mosquitto, the messages are collected by Telegraf and saved in an InfluxDB. Finally the data is displayed by a Grafana dashboard.

# Requirements

- VS Code with the extension 'PlatformIO IDE'
- Docker

# Getting Started

To get started run the setup script, it will create and configure the files to run the containers.

For MacOS please install GNU sed  
`$ brew install gnu-sed`

`$ cd container && ./setup.sh`

## Run the microcontroller

1. `cp src/mqtt_config.template.h src/mqtt_config.h`
2. Set the correct variables in `src/mqtt_config.h`
3. Compile and upload the code onto the microcontroller via the PlatformIO extension

## Run the container enviroment

To start the containers run `./container/start.sh`  
To stop the containers run `./container/stop.sh`

After starting you need to create two Access-Tokens:

1. Open the InfluxDB-UI (http://localhost:8086)
2. Go to Data > Tokens
3. Generate a Write-Token for Telegraf
4. Generate a Read-Token for Grafana
5. Copy and save each token in `container/config.env` (see table below)
6. Restart the containers `./container/start.sh`

| #   | env-variable                | application | permission |
| --- | --------------------------- | ----------- | ---------- |
| 1   | INFLUXDB_ACCESS_TOKEN_WRITE | Telegraf    | write      |
| 2   | INFLUXDB_ACCESS_TOKEN_READ  | Grafana     | read       |

## Configure InfluxDB

- https://docs.influxdata.com/influxdb/cloud/security/tokens/create-token/
- https://docs.influxdata.com/influxdb/v2.0/reference/syntax/line-protocol/

## Configure Grafana

TODO

## mosquitto_passwd

Mosquitto uses a passwordfile in which the usernames and passwords are defined. The passwords must be hashed, in order to hash them follow these steps:

1. Start a mosquitto container
2. Connect to the container CLI
3. Create a passwordfile:  
   `$ echo 'username:password' > passwordfile`
4. Convert the passwords:  
   `$ mosquitto_passwd -U passwordfile`
5. Copy the converted passwords into the workspace

Note: This is already done in the `setup.sh` script.

Reference: https://mosquitto.org/man/mosquitto_passwd-1.html

## Raspberry Pi Setup

- Securing the Raspberry Pi: https://www.raspberrypi.org/documentation/computers/configuration.html#securing-your-raspberry-pi
- Install Docker (armhf): https://docs.docker.com/engine/install/debian/
- To use docker without sudo execute the following steps:
   ```sh
   # Add the group 'docker' and add the user
   $ sudo groupadd docker
   $ sudo usermod -aG docker $USER
   # List the groups (reconnect required)
   $ groups
   ```

## Resources

- https://github.com/Nilhcem/home-monitoring-grafana
- https://dzone.com/articles/raspberry-pi-iot-sensors-influxdb-mqtt-and-grafana
- https://www.influxdata.com/blog/running-influxdb-2-0-and-telegraf-using-docker/
