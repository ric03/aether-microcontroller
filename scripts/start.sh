#!/bin/bash
set -e

cd container/

PROJECT_NAME=aether

echo 'Starting containers...'
docker compose --project-name $PROJECT_NAME up --detach
docker compose --project-name $PROJECT_NAME ps
echo 'containers are up and running.'
