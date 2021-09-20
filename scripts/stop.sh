#!/bin/bash
set -e

PROJECT_NAME=aether

PARTIAL_VOLUME_NAME=influxdb-data
VOLUME_NAME=${PROJECT_NAME}_${PARTIAL_VOLUME_NAME}

echo 'Volumes are persistent and are not destroyed automatically.'
read -p "Would you like to remove the volume $VOLUME_NAME (y/N)? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    # docker volume rm $VOLUME_NAME
    REMOVE_VOLUMES=--volumes
fi

docker-compose --project-name $PROJECT_NAME down $REMOVE_VOLUMES
