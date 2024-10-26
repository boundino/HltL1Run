make drawdigi || exit 1

# INPUT=/eos/cms/store/group/phys_heavyions/wangj/Forest2023/CRAB_UserFiles/crab_forestadc_231001_PhysicsHIForward0_374596.root
INPUT=/eos/cms/store/group/phys_heavyions/wangj/Forest2023/CRAB_UserFiles/crab_forestadc_231003_PhysicsHIForward0_374596_fixZDC.root
OUTPUT=${INPUT##*/}
OUTPUT=${OUTPUT%%.*}

# echo $OUTPUT

# ./drawdigi $INPUT $OUTPUT 6470000 6470054
./drawdigi $INPUT $OUTPUT 454000 454050 
