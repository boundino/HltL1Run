#!/bin/bash

if [[ $# -ne 5 ]]; then
    echo "usage: ./loop-condor-checkfile.sh [input dir] [input forest] [output dir] [max jobs] [log dir]"
    exit 1
fi

DATASETs=$1
FOREST=$2
DESTINATION=$3
MAXFILES=$4
LOGDIR=$5

PROXYFILE=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')

NAME="evtmatch"

SRM_PREFIX="/mnt/hadoop/"
SRM_PATH=${DESTINATION#${SRM_PREFIX}}

[[ ! -d $DESTINATION ]] && {
    gfal-mkdir -p gsiftp://se01.cmsaf.mit.edu:2811/${SRM_PATH} ;
}

mkdir -p $LOGDIR

counter=0
for DATASET in `echo $DATASETs`
do

    [[ -f filelist.txt ]] && rm filelist.txt
    ls $DATASET | grep -v "/" | grep -v -e '^[[:space:]]*$' | awk '{print "" $0}' >> filelist.txt

    for i in `cat filelist.txt`
    do
        [[ $counter -ge $MAXFILES ]] && break
        if [ ! -f ${DESTINATION}/${NAME}_$i ] && [ -f ${DATASET}/$i ]
        then
            if [ -s ${DATASET}/$i ] || [ $ifCHECKEMPTY -eq 0 ]
            then
                echo -e "\033[38;5;242mSubmitting a job for output\033[0m ${DESTINATION}/${NAME}_$i"
                infn=`echo $i | awk -F "." '{print $1}'`
                INFILE="${DATASET}/$i"
	        cat > ${NAME}.condor <<EOF

Universe     = vanilla
Initialdir   = $PWD/
Notification = Error
Executable   = $PWD/loop-evtmatch.sh
Arguments    = $INFILE $FOREST $DESTINATION ${NAME}_${infn}.root $PROXYFILE
GetEnv       = True
Output       = $LOGDIR/log-${infn}.out
Error        = $LOGDIR/log-${infn}.err
Log          = $LOGDIR/log-${infn}.log
Rank         = Mips
+AccountingGroup = "group_cmshi.$(whoami)"
requirements = GLIDEIN_Site == "MIT_CampusFactory" && BOSCOGroup == "bosco_cmshi" && HAS_CVMFS_cms_cern_ch && BOSCOCluster == "ce03.cmsaf.mit.edu"
job_lease_duration = 240
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = evtmatch.exe,/tmp/$PROXYFILE

Queue 
EOF

condor_submit ${NAME}.condor -name submit.mit.edu
mv ${NAME}.condor $LOGDIR/log-${infn}.condor
counter=$(($counter+1))	
            fi
        fi
    done
done
echo -e "Submitted \033[1;36m$counter\033[0m jobs to Condor."
