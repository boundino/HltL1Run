#!/bin/bash

nentries=-1
forests=(
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023/CRAB_UserFiles/crab_forestadc_231012_PhysicsHIPhysicsRawPrime0-2_375055_ZB.root
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023/CRAB_UserFiles/crab_forestadc_231011_PhysicsHIPhysicsRawPrime0-2_374970_ZB.root
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023/CRAB_UserFiles/crab_forestadc_231006_PhysicsHIPhysicsRawPrime0-4_374803_ZB.root
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023/CRAB_UserFiles/crab_forestadc_231005_PhysicsHIPhysicsRawPrime0-8_374778_ZB.root
    # /eos/cms/store/group/phys_heavyions/wangj/Forest2023/CRAB_UserFiles/crab_forestadc_231003_PhysicsHIForward0_374596_fixZDC.root
    # /eos/cms/store/group/phys_heavyions/wangj/L1PbPb2022/crab_adcRAW_221124_HITestRaw0_HIRun2022A_ZBPVfil_362294-6.root
    /afs/cern.ch/work/w/wangj/RECO2023/forest/CMSSW_13_2_5_patch1/src/crab_forestadc.root
)

make evtmatch_samefile || exit 1

for FOREST in ${forests[@]}
do
    OUTPUT=${FOREST/crab_/evtmatching_}
    echo $OUTPUT
    ./evtmatch_samefile $FOREST $OUTPUT $nentries
done


# tmp=$(date +%y%m%d%H%M%S)
# rm evtmatch_samefile_${tmp}
