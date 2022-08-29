#!/bin/bash

FOREST=/raid5/data/wangj/L1PbPb2022/crab_HiForestADC_20220828_HydjetDrum5F_5p02TeV_122X_2021_realistic_HI_v10.root
OUTPUT=/raid5/data/wangj/L1PbPb2022/evtmatching_HiForestADC_20220828_HydjetDrum5F_5p02TeV_122X_2021_realistic_HI_v10.root

tmp=$(date +%y%m%d%H%M%S)

g++ evtmatch_samefile.cc $(root-config --libs --cflags) -g -o evtmatch_samefile_${tmp}.exe || exit 1

./evtmatch_samefile_${tmp}.exe $FOREST $OUTPUT

rm evtmatch_samefile_${tmp}.exe
