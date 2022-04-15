#!/usr/bin/env bash

printhelp() {
    printf 'Usage:\n'
    printf "    %s <instance1> <instance2> ...\n" "$0"
}

origdir="$(dirname "${BASH_SOURCE[0]}")"

if [[ "$*" = '' ]]
then
    printf 'Incorrect usage!\n'
    printhelp
    exit 1
else
    for each in "$@"
    do
        if [[ "$each" = '--help' ]]
        then
            printhelp
            exit
        fi
    done
fi

for each in "$@"
do
    if [[ -d ./instances/$each ]]
    then
        cd ./instances/"$each" || exit
        for log in ./.minecraft/logs/*
        do
            mkdir -p .uncleanbackup
            cp "$log" .uncleanbackup
            sed -i '/[eE]ly/d' "$log"
            sed -i '/[Aa]uth[Ll]ib/d' "$log"
            sed -i '/400/d' "$log"
            sed -i 's/UltimMC/MultiMC/g' "$log"
            sed -i 's/ultim/multi/g' "$log"
        done
        cd "$origdir" || exit
    else
        printf "Instance '%s' does not exist!\n" "$each"
        exit 1
    fi
done
