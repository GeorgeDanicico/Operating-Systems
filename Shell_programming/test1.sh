#!/bin/bash

if test $# -lt 3; then
	echo "invalid arguments"
	exit 1
fi

value=$#

if ! test $((value%3)) -eq 0; then
	echo "one pair will be incomplete!"
fi

while test $# -gt 2
do
	file="$1"
	words=$2
	line=$3
	if ! test -f "$file"; then
		echo "$file is not a file."
	else
		count=$(cat $file | awk -v var=$line 'NR==var{print NF}')
		count2=$(head --lines=$line $file | tail --lines=1 | awk '{print NF}')
		if test $count2 -eq $words; then
			echo "There is in the file $file on line $line $words words."
		fi

	fi


	shift 3
done

if ! test $# -eq 0; then
	echo "Warning, one pair is incomplete"
fi

