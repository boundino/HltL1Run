#!/bin/bash

if [[ $# -ne 4 ]]; then
    echo "usage: ./loop-skim.sh [input file] [output dir] [output filename] [proxy]"
    exit 1
fi

INFILE=$1
DESTINATION=$2
OUTFILE=$3
export X509_USER_PROXY=${PWD}/$4

SRM_PREFIX="/mnt/hadoop/"
SRM_PATH=${DESTINATION#${SRM_PREFIX}}

set -x
./skim.exe $INFILE $OUTFILE
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