#!/bin/bash

print_usage() {
    cat <<EOF
Read live car directions from Apache2 server running on the BBB

Usage:
   $0 <IP>
EOF
}

if [[ ${#@} -ne 1 ]]
then
    print_usage
    exit 1
fi

while true
do
    say `curl -s http://$1/mr_robot/info.txt | grep Direction | sed 's/Direction: //g'` 
    sleep 2
done
