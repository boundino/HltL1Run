#!/bin/bash

skim=0
ifs=(0) # input directory

## ifs
inputdirs=(
    /export/d00/scratch/jwang/L1PbPb2021/crab_L1_20200322_HIZeroBiasReducedFormat_HIRun2018A_v1_2/ # 0
)

########################################
## >>> do not change lines below >>>  ##
########################################

tmp=$(date +%y%m%d%H%M%S)
cp merge.C merge_${tmp}.C
g++ merge_${tmp}.C $(root-config --libs --cflags) -g -o merge_${tmp}.exe || exit 1

for i in ${ifs[@]}
do
    [[ $i -lt ${#inputdirs[@]} ]] || break
    inputdir=${inputdirs[i]}
    IFS='/'; subdir=($inputdir); unset IFS;
    request=${subdir[${#subdir[@]}-1]}
    primedir=${inputdir%%${request}*}

    [[ ! -d $inputdir ]] && continue

    ## ======================================== #

    filelist=filelist_${request}.txt
    [[ -f $filelist ]] && {
        # echo "error: filelist $filelist exits. "
        # echo "remove filelist? (y/n):"
        # rewrite=
        # while [[ $rewrite != 'y' && $rewrite != 'n' ]]
        # do
        #     read rewrite
        #     if [[ $rewrite == 'y' ]] ; then { rm $filelist ; } ;
        #     elif [[ $rewrite == 'n' ]] ; then { echo "please change output file name" ; rm merge_${tmp}.exe ; continue ; } ;
        #     else { echo "please input y/n" ; } ; fi ;
        # done
        rm $filelist
    } 

    ls $inputdir/*.root -d > $filelist

        set -x
        output=${primedir}/${request}_merge.root
        set +x
        willrun=1
        [[ -f $output ]] && {
            echo "error: output $output exits. "
            echo "remove output? (y/n):"
            rewrite=
            while [[ $rewrite != 'y' && $rewrite != 'n' ]]
            do
                read rewrite
                if [[ $rewrite == 'y' ]] ; then { echo "$output removed" ; rm $output ; } ;
                elif [[ $rewrite == 'n' ]] ; then { echo "please change output file name" ; willrun=0 ; } ;
                else { echo "please input y/n" ; } ; fi ;
            done
        }

        [[ $willrun -eq 0 ]] && continue
        [[ ${1:-0} -eq 1 ]] && { ./merge_${tmp}.exe $output $filelist $skim ; }
done

rm merge_${tmp}.exe
rm merge_${tmp}.C
