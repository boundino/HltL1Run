

g++ hfBins.cc $(root-config --libs --cflags) -I$HOME -g -o hfBins.exe || exit 1

# INPUT=/raid5/data/wangj/L1PbPb2022/crab_HiForest_20221120_MB_Hydjet_Run3_subehera_wADC_v2.root
# SEL='1'
# OUTPUT=MiniAOD_Hydjet_Run3_subehera
# ./hfBins.exe $INPUT "$SEL" $OUTPUT "hiEvtAnalyzer/HiTree::hiHF"


INPUT=/raid5/data/wangj/L1PbPb2022/crab_adcRAW_221124_HITestRaw0_HIRun2022A_ZBPVfil_362294-6.root
SEL='nhfp > 1 && nhfn > 1'
OUTPUT=HITestRaw0_ZB2Th4
./hfBins.exe $INPUT "$SEL" $OUTPUT 


