#!/bin/bash

files=$(find "$1" -type f)
for file in $files
do
	f=$(ls -all $file | grep -E "^..w..w..w" | awk '{print $1, $9}')
	if test -n "$f"; then
		echo $f
		chmod o-w $file
		echo $(ls -all $file | awk '{print $1, $9}')
	fi


done
