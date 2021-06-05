#!/bin/bash

declare -A array

if test $# -lt 1; then
	echo "Invalid arguments."
	exit 1
fi

for elem in $@
do
	array[$elem]=0
done
zero=0

while true
do
	F=""
	read -p "Provide a file: " F
	
	
	if test -f "$F"; then
		for key in "${!array[@]}"
		do
			if test ${array[$key]} -eq 0 && grep -E -q "\<$key\>" "$F"; then
				echo "$key found in file $F."
				array[$key]=1
				zero=`expr $zero + 1`	
			fi
		done 
		if test $zero -eq $#; then
			echo "All words have been found!"
			exit 1
		fi
	fi


done
