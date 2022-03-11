#!/bin/bash

filelist=filelist.txt
g++ skimbranch.cc $(root-config --libs --cflags) -I"$HOME/" -g -o skimbranch.exe || exit 1

for input in `cat $filelist`
do
    output=${input##*/}
    output="output/skim_"$output
    # echo $input      $output
    ./skimbranch.exe $input $output
done

rm skimbranch.exe

echo '====> Done <===='
