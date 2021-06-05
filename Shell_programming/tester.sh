#!/bin/bash


IFS=$'\n'
for elem in $(df -BM | awk '{print $0}')
do
	echo $elem

done
