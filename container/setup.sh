#!/bin/bash
set -e

MOSQUITTO_IMAGE_NAME=eclipse-mosquitto:2.0.10
CONTAINER_NAME=moquitto-setup-cli
MOSQUITTO_PASSWD_UNENCRYPTED=config/mosquitto.passwd.raw
MOSQITTO_PASSWD_ENCRYPTED=config/mosquitto.passwd

echo "Copying required templates..."
cp config.env.template              config.env
cp config/mosquitto.passwd.template $MOSQUITTO_PASSWD_UNENCRYPTED
echo 'created: config.env'
echo "created: $MOSQUITTO_PASSWD_UNENCRYPTED"

echo "Please provide passwords for the Mosquitto users..."
read -sp "Sensor password: " sensor-password
read -sp "Telegraf password: " telegraf-password

echo "Configuring mosquitto passwords..."
sed 's/sensor-password/${sensor-password}' $MOSQUITTO_PASSWD_UNENCRYPTED
sed 's/telegraf-password/${telegraf-password}' $MOSQUITTO_PASSWD_UNENCRYPTED
echo "Wrote raw passwords into $MOSQUITTO_PASSWD_UNENCRYPTED"


trap "docker rm --force $CONTAINER_NAME" SIGHUP SIGINT SIGQUIT SIGABRT SIGKILL SIGTERM

echo "Encrypting mosquitto passwords..."
docker run  --name $CONTAINER_NAME -itd $MOSQUITTO_IMAGE_NAME
docker cp   $MOSQUITTO_PASSWD_UNENCRYPTED $CONTAINER_NAME:mosquitto.passwd
docker exec $CONTAINER_NAME mosquitto_passwd -U mosquitto.passwd
docker cp   $CONTAINER_NAME:mosquitto.passwd $MOSQITTO_PASSWD_ENCRYPTED
docker rm   --force $CONTAINER_NAME
echo "Wrote encrypted passwords into $MOSQITTO_PASSWD_ENCRYPTED"

read -p "Would you like to remove the local image $MOSQUITTO_IMAGE_NAME (y/n)? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    docker rmi $MOSQUITTO_IMAGE_NAME
fi

echo "Setup complete. Please refer to the README for the next steps."
