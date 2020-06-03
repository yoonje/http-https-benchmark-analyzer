#!/bin/bash

if [ -z $1 ]; then
        echo " $ ./http-https-benchmarker.sh [address]"
        exit 1;
fi

# compile
gcc -o http_client http_client.c
gcc -c -o https_client.o https_client.c
gcc -o https_client https_client.o -lssl -lcrypto

# http
StartTime=$(date +%s)
http_client $1
EndTime=$(date +%s)
echo "HTTP takes $(($EndTime - $StartTime)) seconds to complete this task."

# https
StartTime=$(date +%s)
https_client $1
EndTime=$(date +%s)
echo "HTTPS takes $(($EndTime - $StartTime)) seconds to complete this task."