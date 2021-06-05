#!/bin/bash

users=$(awk '{print $1}' who.fake)

for user in $users
do
	p=$(grep "^\<$user\>" ps.fake | wc -l)
	echo "$user $p"
done
