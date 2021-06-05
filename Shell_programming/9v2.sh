#!/bin/bash

declare -A files

if ! test -d "$1"; then
	echo "$1 is not a directory"
	exit 1
fi

b=""
for file in $(find "$1" -type f)
do
	if test -f "$file"; then
		key=$(md5sum "$file" | awk '{print $1}')
		files[$key]="${files[$key]}$file "
		b=$key
	fi
done

for key in ${!files[@]}
do
	count=$(echo ${files[$key]} | wc -w)
	if test $count -gt 1; then
		echo $key ${files[$key]}
	fi	
done
