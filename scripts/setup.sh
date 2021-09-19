#!/bin/bash
set -e

echo 'Container config setup'
echo '---------------------'
cd container/

MOSQUITTO_IMAGE_NAME=eclipse-mosquitto:2.0.12
CONTAINER_NAME=moquitto-setup-cli
MOSQUITTO_PASSWD_UNENCRYPTED=config/mosquitto.passwd.raw
MOSQITTO_PASSWD_ENCRYPTED=config/mosquitto.passwd

echo "Copying required templates..."
cp config.env.template              config.env
cp config/mosquitto.passwd.template $MOSQUITTO_PASSWD_UNENCRYPTED
echo '+ created: config.env'
echo "+ created: $MOSQUITTO_PASSWD_UNENCRYPTED"

echo "Please provide passwords for the Mosquitto users..."
read -sp "Sensor password: " sensorPassword
echo
read -sp "Telegraf password: " telegrafPassword
echo

echo "Configuring mosquitto passwords..."

if [ $(uname) = Darwin ]
then 
    SED=gsed
else 
    SED=sed
fi

$SED -i "s/sensorPassword/$sensorPassword/" $MOSQUITTO_PASSWD_UNENCRYPTED
$SED -i "s/telegrafPassword/$telegrafPassword/" $MOSQUITTO_PASSWD_UNENCRYPTED
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
