#!/bin/bash

files=$(awk 'NR>1{print $6}' df.fake)
for file in $files
do
	i=$(grep -E "$file$" df.fake | awk '{print $4, $5}')
	memory=$(echo $i | awk '{print $1}' | sed -E "s/([0-9]*)./\1/gi")
	overload=$(echo $i | awk '{print $2}' | sed -E "s/([0-9]*)./\1/gi")	

	if test $memory -lt 1000 || test $overload -gt 80; then
		echo $file
	fi

done
