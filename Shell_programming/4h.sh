#!/bin/bash

for file in $@
do
	if test -f "$file"; then
		echo $(ls -l $file) >> f
	else
		echo $file is not a file
	fi

done

cat f | awk '{print $5, $9}' | sort -n -r
rm f

