#!/bin/bash

# if no programs entered

while true
do
	for P in $@
	do
		PID=$(ps -ef | awk '{print $2, $9 }' | grep -E "$P" | awk '{print $1}')
		
		if test -n "$PID"; then
			kill -9 $PID
			echo "$P has been killed."
			sleep 2	
		fi		

	done	

done
