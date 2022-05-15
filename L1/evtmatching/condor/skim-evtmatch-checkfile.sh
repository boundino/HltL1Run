#!/bin/bash

if [[ $# -ne 5 ]]; then
    echo "usage: ./skim-evtmatch-checkfile.sh [input file] [input file L1] [output dir] [output filename] [proxy]"
    exit 1
fi

INFILE=$1
INFILEL1=$2
DESTINATION=$3
OUTFILE=$4
export X509_USER_PROXY=${PWD}/${5}
echo $X509_USER_PROXY

#FILE=$(head -n$(($1+1)) $2 | tail -n1)
# tar -xzvf mva.tgz

set -x
./evtmatch_inverse.exe $INFILE $INFILEL1 $OUTFILE -1

if [[ $(wc -c $OUTFILE | awk '{print $1}') -gt 500 ]]; then
    SRM_PREFIX="/mnt/T2_US_MIT/hadoop/"
    SRM_PATH=${DESTINATION#${SRM_PREFIX}}
    LD_LIBRARY_PATH='' PYTHONPATH='' gfal-copy file://$PWD/${OUTFILE} gsiftp://se01.cmsaf.mit.edu:2811/${SRM_PATH}/${OUTFILE}
    # cp $OUTFILE $DESTINATION/
fi
set +x

rm $OUTFILE
