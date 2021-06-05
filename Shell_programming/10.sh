#!/bin/bash

month=$(date +%b)

length=$(last | grep -E "" | wc -l)
last | head --lines=$((length-2)) >> f


grep -E "\<Mar\>" f | awk '{print $1}' | sort -r | uniq -c
rm f
