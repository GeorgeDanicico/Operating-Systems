#!/bin/bash

find "$1" -not -empty -type f -exec md5sum {} + | sort | uniq -w32 -dD
