#!/bin/bash

nentries=-1
# /eos/cms/store/group/phys_heavyions/wangj/Forest2024/CRAB_UserFiles/crab_forestadc_HIPhysicsRawPrime0_387867_ZB.root
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
