#!/bin/bash

input=/eos/cms/store/group/phys_heavyions_ops/L1Emulator/L1AndOfflineEvtMatch/L1NtupleRAWEMUCalo_HFAdc_smallStat/output/EvtL1CaloInfo_327078_50.root
output=skim_EvtL1CaloInfo_327078_50.root

g++ skimbranch.cc $(root-config --libs --cflags) -I"$HOME/" -g -o skimbranch.exe || exit 1

./skimbranch.exe $input $output

rm skimbranch.exe

echo '====> Done <===='
