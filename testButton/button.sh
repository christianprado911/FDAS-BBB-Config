#!/bin/bash

	/usr/bin/config-pin p9.19 gpio

echo "out" > /sys/class/gpio/gpio13/direction

i=0
	while [ "$i" -le 50 ]
	do
		echo 1 > /sys/class/gpio/gpio13/value
		sleep 0.3s
		echo 0 > /sys/class/gpio/gpio13/value
		sleep 0.3s
		i=$(( i+1 ))
	done
