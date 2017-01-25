#!/bin/bash

print_usage() {
 cat<<EOF
Google Speech Recognition API - SETUP

Usage:
    source setup.sh /path/to/your/credentials-key.json
EOF
}

export_env() {
    if [[ -z "$1" ]] || [ "$(echo "$0" | awk '{print $1}')" != "bash" ]
    then
        print_usage
    else
        export GOOGLE_APPLICATION_CREDENTIALS="$1"
        echo SUCCESS!
        echo Now you can use the bin scripts!!!
    fi
}

export_env $@
