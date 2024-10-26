#!/bin/bash

comp="$(root-config --libs --cflags) -I$HOME"
[[ $# == 0 || ${1:-0} -eq 1 ]] && { g++ adcToGeV.C $comp -g -o adcToGeV.exe || exit 1 ; }
[[ $# == 0 || ${2:-0} -eq 1 ]] && { g++ drawadcToGeV.C $comp -g -o drawadcToGeV.exe || exit 1 ; }

inputs=(
    configs/run373870.conf
)

for ii in ${inputs[@]}
do
    [[ ${1:-0} -eq 1 ]] && ./adcToGeV.exe $ii
    [[ ${2:-0} -eq 1 ]] && ./drawadcToGeV.exe $ii

done

rm *.exe
