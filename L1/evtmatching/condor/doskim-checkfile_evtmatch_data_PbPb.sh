#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

#
MAXFILENO=10

#
dccfile="evtmatch_inverse.cc"
#
movetosubmit=${1:-0}
runjobs=${2:-0}


INPUTS=(
    "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HIForward/crab_forest_20220512_HIForward_HIRun2018A_HIZeroBiasv1_run327327/220512_232708/0000/,root://cms-xrd-global.cern.ch//store/user/jwang/L1Run2022/crab_l1Ntuple_20220428_HIForward_HIRun2018A_shuaiySkimedZB_run327327.root"
)

WORKDIR="/work/submit/$USER/l1evtmatch/"
mkdir -p $WORKDIR
OUTPUTVER=""

for INP in "${INPUTS[@]}"
do
    IFS=','; INPS=(${INP}); unset IFS;
    INPUTDIR=${INPS[0]}
    INPUTL1=${INPS[1]}
    
    echo -e "\e[32;1m$INPUTDIR\e[0m"

    if [[ $INPUTDIR == *.txt ]]
    then
        INPUTFILELIST=$INPUTDIR        
    else
        CRABNAME=${INPUTDIR##*crab_} ; CRABNAME=${CRABNAME%%/*} ;
        INPUTFILELIST="$WORKDIR/filelists/filelist_"$CRABNAME".txt"
        ls --color=no $INPUTDIR/* -d | sed -e "s/\/mnt\/T2_US_MIT\/hadoop\/cms/root:\/\/cms-xrd-global.cern.ch\//g" > $INPUTFILELIST
        
    fi
    echo "$INPUTFILELIST"
    REQUESTNAME=${INPUTFILELIST##*filelist_} ; REQUESTNAME=${REQUESTNAME%%.txt} ;
    OUTPUTSUBDIR="evtmatching_${REQUESTNAME}$OUTPUTVER"
    OUTPUTPRIDIR="/mnt/T2_US_MIT/hadoop/cms/store/user/jwang/L1Run2022/"
    # OUTPUTPRIDIR="/mnt/T2_US_MIT/hadoop/cms/store/user/jwang/DntupleRun2018condor/"
    # OUTPUTPRIDIR="/mnt/submit-hi1/d00/scratch/jwang/DntupleRun2018/"

    ##
    OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
    LOGDIR="logs/log_${OUTPUTSUBDIR}"

    echo "$OUTPUTDIR"
    ##

    if [ "$runjobs" -eq 1 ]
    then 
        set -x
        ./skim-condor-checkfile.sh "$INPUTFILELIST" $INPUTL1 $OUTPUTDIR $MAXFILENO $LOGDIR 
        set +x
    fi

done

if [ "$movetosubmit" -eq 1 ]
then
    cd ../
    g++ $dccfile $(root-config --cflags --libs) -g -o evtmatch_inverse.exe || exit 1
    cd condor

    mv -v ../evtmatch_inverse.exe $WORKDIR/
    cp -v $0 $WORKDIR/
    cp -v skim-evtmatch-checkfile.sh $WORKDIR/
    cp -v skim-condor-checkfile.sh $WORKDIR/
fi

