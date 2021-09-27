# Project Aether

Measure and display the air quality.

This project measures the air quality with a microcontroller, the data is sent via MQTT to Mosquitto, the messages are collected by Telegraf and saved in an InfluxDB. Finally the data is displayed by a Grafana dashboard.

# Requirements

- VS Code with the extension 'PlatformIO IDE'
- Docker

# Getting Started

To get started run the setup script, it will create and configure the files to run the containers.

`$ ./scripts/setup.sh`


## Run the microcontroller

1. `cp src/mqtt_config.template.h src/mqtt_config.h`
2. Set the correct variables in `src/mqtt_config.h`
3. Compile and upload the code onto the microcontroller via the PlatformIO extension

## Run the container enviroment

To start the containers run `./scripts/start.sh`  
To stop the containers run `./scripts/stop.sh`

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
   restart is required `shutdown -r 0`
- To use docker without sudo execute the following steps:
   ```sh
   # Add the group 'docker' and add the user
   $ sudo groupadd docker
   $ sudo usermod -aG docker $USER
   # List the groups (reconnect required)
   $ groups
   ```
- Install docker-compose:
   ```sh
   sudo apt-get install libffi-dev libssl-dev
   sudo apt install python3-dev
   sudo apt-get install -y python3 python3-pip
   sudo pip3 install docker-compose
   ```

## Resources

- https://github.com/Nilhcem/home-monitoring-grafana
- https://dzone.com/articles/raspberry-pi-iot-sensors-influxdb-mqtt-and-grafana
- https://www.influxdata.com/blog/running-influxdb-2-0-and-telegraf-using-docker/
