#!/bin/bash

# FOREST="/export/d00/scratch/jwang/L1ppLowPU2018/matchingl1forest/crab_HiForestAOD_20180730_lowPUpp_ZeroBias_Run319460.root"
# L1="/export/d00/scratch/jwang/L1ppLowPU2018/matchingl1forest/crab_L1_20180730_lowPUpp_ZeroBias_Run319460.root"
# OUTPUT="/export/d00/scratch/jwang/L1ppLowPU2018/matchingl1forest/crab_HiForestAOD_match_L1_20180730_lowPUpp_ZeroBias_Run319460.root"
# nentries=1000000
# FOREST="/export/d00/scratch/jwang/L1PbPb2021/crab_HiForestAOD_20200323_HIZeroBiasReducedFormat_reReco.root"
# L1="/export/d00/scratch/jwang/L1PbPb2021/skim_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2.root"
# OUTPUT="/export/d00/scratch/jwang/L1PbPb2021/match_HiForestAOD_L1_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_evt1000000.root"
# L1="/export/d00/scratch/jwang/L1PbPb2021/L1Ntuple_246.root"
# OUTPUT="/export/d00/scratch/jwang/L1PbPb2021/match_20190325_HIZeroBiasReducedFormat_HIRun2018A_v1_L1Ntuple_246.root"

nentries=-1
FOREST=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/crab_forest_20220501_HIForward_HIRun2018A_HIZeroBiasv1_run327238.root
L1=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/crab_l1Ntuple_20220428_HIForward_HIRun2018A_shuaiySkimedZB_run327238.root
OUTPUT=/eos/cms/store/group/phys_heavyions/wangj/L1PbPb2018/evtmatching_20220501_HIForward_HIRun2018A_HIZeroBiasv1_run327238.root

tmp=$(date +%y%m%d%H%M%S)

g++ evtmatch.cc $(root-config --libs --cflags) -g -o evtmatch_${tmp}.exe || exit 1

[[ ! -f $FOREST || ! -f $L1 ]] && { echo "no file: $FOREST $L1" ; exit 1 ; }
./evtmatch_${tmp}.exe $FOREST $L1 $OUTPUT $nentries

rm evtmatch_${tmp}.exe
