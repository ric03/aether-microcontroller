echo 'Removing container config...'
rm container/config.env
rm container/config/mosquitto.passwd
rm container/config/mosquitto.passwd.raw
echo '...done'

read -p "Would you like to remove the μController config (y/N)? " -n 1
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo 'Removing μController config'
    rm mqtt_config.h
    echo '...done'
fi

