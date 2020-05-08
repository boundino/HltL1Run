#!/bin/bash

[[ $0 != *.sh ]] && {
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m" ; return 1; }

# ===> Please check the info below
MAXFILENO=5
OUTPUTSUBDIR=
# <=== 

INPUTS=(
    '/mnt/hadoop/cms/store/user/wangj/HIZeroBiasReducedFormat/crab_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2/200323_025913/0000/ evtmatch_L1_20200410_HIZeroBiasReducedFormat_HIRun2018A_v1_2'
)
# INPUTFOREST=/mnt/hadoop/cms/store/user/jwang/L1Run2021condor/crab_HiForestAOD_20200323_HIZeroBiasReducedFormat_reReco.root
INPUTFOREST=/mnt/submit-hi2/d00/scratch/jwang/L1PbPb2021/crab_HiForestAOD_20200323_HIZeroBiasReducedFormat_reReco.root

##############################################################
# Do not modify lines below if you do not know what it means #
##############################################################

##
movetosubmit=${1:-0}
runjobs=${2:-0}

[[ $movetosubmit -eq 0 && $runjobs -eq 0 ]] && {
    echo "Usage: ./submit_condor_checkfile.sh [move script] [submit jobs]" ;
    echo "Test compiling macros..." ; 
    g++ evtmatch.cc $(root-config --libs --cflags) -I"/afs/lns.mit.edu/user/jwang" -g -o evtmatch.exe || exit 1 && rm evtmatch.exe
}

#
OUTPUTPRIDIR="/mnt/hadoop/cms/store/user/jwang/L1Run2021condor/"
WORKDIR="/work/$USER/L1Run2021match/"
mkdir -p $WORKDIR

#
if [[ $movetosubmit -eq 1 ]]
then
    if [[ $(hostname) == "submit-hi2.mit.edu" || $(hostname) == "submit.mit.edu" || $(hostname) == "submit-hi1.mit.edu" ]]
    then
        g++ evtmatch.cc $(root-config --libs --cflags) -I"/afs/lns.mit.edu/user/jwang" -g -o evtmatch.exe

        mv evtmatch.exe $WORKDIR/
        cp $0 $WORKDIR/
        cp loop-evtmatch.sh $WORKDIR/
        cp loop-condor-checkfile.sh $WORKDIR/
        echo $WORKDIR
    else
        echo -e "\e[31;1merror:\e[0m compile macros on \e[32;1msubmit-hiX.mit.edu\e[0m or \e[32;1msubmit.mit.edu\e[0m."
    fi
fi

#
if [[ $runjobs -eq 1 ]]
then
    if [[ $(hostname) == "submit.mit.edu" ]]
    then
##
        if [[ x$OUTPUTSUBDIR == x ]]
        then
            for ii in "${INPUTS[@]}"
            do
                thisinput=($ii)
                INPUTDIR=${thisinput[0]}
                OUTPUTSUBDIR=${thisinput[1]}
                OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
                LOGDIR="logs/log_${OUTPUTSUBDIR}"
                echo "./loop-condor-checkfile.sh \"$INPUTDIR\" $INPUTFOREST $OUTPUTDIR $MAXFILENO $LOGDIR"
                ./loop-condor-checkfile.sh "$INPUTDIR" $INPUTFOREST $OUTPUTDIR $MAXFILENO $LOGDIR
            done
        else
            OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
            LOGDIR="logs/log_${OUTPUTSUBDIR}"
            echo "./loop-condor-checkfile.sh \"$INPUTDIR\" $INPUTFOREST $OUTPUTDIR $MAXFILENO $LOGDIR"
            ./loop-condor-checkfile.sh "$INPUTDIR" $INPUTFOREST $OUTPUTDIR $MAXFILENO $LOGDIR
        fi
    else
        echo -e "\e[31;1merror:\e[0m submit jobs on \e[32;1msubmit.mit.edu\e[0m."
    fi
fi
