#!/bin/bash

make adcToGeV drawadcToGeV 

conf=$1

[[ $conf == *conf && -f $conf ]] && {
    [[ ${2:-0} -eq 1 ]] && ./adcToGeV $conf
    [[ ${3:-0} -eq 1 ]] && ./drawadcToGeV $conf
}

