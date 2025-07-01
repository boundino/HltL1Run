#!/bin/bash

make adcToGeV drawadcToGeV hfchannel

conf=$1

[[ $conf == *conf && -f $conf ]] && {
    [[ ${2:-0} -eq 1 ]] && ./adcToGeV $conf
    [[ ${3:-0} -eq 1 ]] && ./drawadcToGeV $conf
    [[ ${4:-0} -eq 1 ]] && ./hfchannel $conf
}

