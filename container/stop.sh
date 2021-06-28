#!/bin/bash
set -e

PROJECT_NAME=aether

docker compose --project-name $PROJECT_NAME down

echo 'containers are stopped. If you want to remove them run: ./remove.sh'
