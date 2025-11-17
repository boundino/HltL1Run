filepath="${1:-/eos/cms/store/group/phys_heavyions/wangj/RECO2023/miniaod_PhysicsHIForward0_374596}"
outputfile=${filepath/'*'/''}
outputfile=${outputfile##*/}.txt

echo $outputfile
rm $outputfile 2> /dev/null

for i in `ls -d $filepath/*`
do
    infile=${i/'/eos/cms'/}
    echo $infile >> $outputfile
done

