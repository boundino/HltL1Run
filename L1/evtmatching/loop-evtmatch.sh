#!/bin/bash

if [[ $# -ne 5 ]]; then
    echo "usage: ./loop-evtmatch.sh [input file] [input forest] [output dir] [output filename] [proxy]"
    exit 1
fi

INFILE=$1
INFILEFOREST=$2
DESTINATION=$3
OUTFILE=$4
export X509_USER_PROXY=${PWD}/$5

SRM_PREFIX="/mnt/hadoop/"
SRM_PATH=${DESTINATION#${SRM_PREFIX}}

set -x
./evtmatch.exe $INFILE $INFILEFOREST $OUTFILE
set +x

echo $(which python)
echo $PYTHONHOME
echo $CMSSW_VERSION
echo $SCRAM_ARCH
echo $LD_LIBRARY_PATH
echo

set -x
if [[ -f $OUTFILE ]]; then
    LD_LIBRARY_PATH='' PYTHONPATH='' gfal-copy file://$PWD/${OUTFILE} gsiftp://se01.cmsaf.mit.edu:2811/${SRM_PATH}/${OUTFILE}
fi
set +x

rm $OUTFILE