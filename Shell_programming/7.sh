#!/bin/bash

string=$(sed -E "s/([^.*$])/\1\@scs.ubbcluj.ro/gi" "$1")

emails=""
for elem in $string
do
	if test -n "$emails"; then
		emails+=","
	fi
		
	emails+=$elem

done

echo $emails
