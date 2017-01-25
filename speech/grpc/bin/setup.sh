#!/bin/bash

print_usage() {
 cat<<EOF
Google Speech Recognition API - SETUP

Usage:
    $0 /path/to/your/credentials-key.json
EOF
}

export_env() {
    if [[ ${#@} < 1 ]]
    then
        print_usage
    else
        export GOOGLE_APPLICATION_CREDENTIALS="$1"
        echo SUCCESS!
        echo Now you can use the bin scripts!!!
    fi
}

export_env "$1"
