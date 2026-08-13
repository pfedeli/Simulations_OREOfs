Ncores=$(grep -c ^processor /proc/cpuinfo)
echo "number of available cores on this computer: $Ncores"

Nthreads=$Ncores-2
#Nthreads=10

./OREOfs macros/run.mac $Nthreads > output/output.out

#nohup sh execute.sh > output/nohup &

