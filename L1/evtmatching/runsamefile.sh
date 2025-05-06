#!/bin/bash

nentries=-1
# /eos/cms/store/group/phys_heavyions/wangj/Forest2024/CRAB_UserFiles/crab_forestadc_HIPhysicsRawPrime0_387867_ZB.root
centrality=`cat mbntuplizer.h | grep '#include "cent/'`
hiBin=`cat mbntuplizer.h | grep 'getHiBinFromhiHF' | sed 's/^[[:space:]]*//g'`
echo -e "\033[1m$centrality\033[0m"
[[ $hiBin == "//"* ]] && echo -e "\033[31m$hiBin\033[0m" || echo -e "\033[32m$hiBin\033[0m"

make evtmatch_samefile || exit 1

[[ $1 == *crab_*.root && -f $1 ]] && {

    FOREST=$1

    OUTPUT=${FOREST/crab_/evtmatching_}
    echo $OUTPUT
    [[ $FOREST == $OUTPUT ]] && { echo "error: output name conversion." ; continue ; }
    ./evtmatch_samefile $FOREST $OUTPUT $nentries

    # tmp=$(date +%y%m%d%H%M%S)
    # rm evtmatch_samefile_${tmp}

}
