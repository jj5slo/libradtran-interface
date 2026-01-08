#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

cp config.conf __config.conf

echo "100M_all start"
cp CONFIGS/1G_all.conf config.conf
./util/runonce/runonce 2022 6 1 3 36  1> /tmp/TEMPORARY/libradtran-interface.log
echo "100M_all finished"

#cp CONFIGS/1M_64-30.conf config.conf
#for i in `seq 1 100`;do
#	echo "Progress: $i,"
#	./main 2022 6 1 3 36  1>  /tmp/TEMPORARY/libradtran-interface.log
#done

