#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

cp config.conf __config.conf

#cp CONFIGS/msisonce.conf config.conf
#	./util/runonce/runonce 2019 01 01 07 50 25 1> /tmp/TEMPORARY/libradtran-interface.log


#for i in `seq 0 23`; do
#	for j in `seq 0 20 59`; do
#		echo "$i $j"
#		cp CONFIGS/sita.conf config.conf
#		./util/runonce/runonce 2022 06 01 "$i" "$j" 36 1> /tmp/TEMPORARY/libradtran-interface.log
#	done
#done
#
#for i in `seq 0 23`; do
#	for j in `seq 0 20 59`; do
#		echo "$i $j"
#		cp CONFIGS/sita2.conf config.conf
#		./util/runonce/runonce 2022 06 01 "$i" "$j" 36 1> /tmp/TEMPORARY/libradtran-interface.log
#	done
#done

#cp CONFIGS/sita2.conf config.conf
#	./util/runonce/runonce 2022 06 01 03 00 36 1> /tmp/TEMPORARY/libradtran-interface.log

#cp CONFIGS/8ji.conf config.conf
#	./util/runonce/runonce 2022 06 01 08 00 15 1> /tmp/TEMPORARY/libradtran-interface.log
#cp CONFIGS/15ji.conf config.conf
#	./util/runonce/runonce 2022 06 01 15 00 15 1> /tmp/TEMPORARY/libradtran-interface.log
#cp CONFIGS/16ji.conf config.conf
#	./util/runonce/runonce 2022 06 01 16 00 15 1> /tmp/TEMPORARY/libradtran-interface.log

cp CONFIGS/20190101_0750_N5.conf config.conf
for i in `seq 1 3`; do
	echo "$i / 3 times repeating"
	
	echo "started"
		./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
	echo "finished"

done

#for i in `seq 1 3`; do
#	echo "$i / 3 times repeating"
#	
#	echo "20190101_0750_N10 started"
#	cp CONFIGS/20190101_0750_N10.conf config.conf
#		./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#	echo "20190101_0750_N10 finished"
#
#	echo "20190101_0750_N5 started"
#	cp CONFIGS/20190101_0750_N5.conf config.conf
#		./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#	echo "20190101_0750_N5 finished"
#
#done


#for i in `seq 1 3`; do
#	echo "$i / 3 times repeating"
#
#	echo "64-29_1M_N2 start"
#	cp CONFIGS/64-29_1M_N2.conf config.conf
#		./main 2022 6 1 3 36  1> /tmp/TEMPORARY/libradtran-interface.log
#	echo "64-29_1M_N2 finished"
#	
#	
#	echo "64-25_1M_N10 start"
#	cp CONFIGS/64-25_1M_N10.conf config.conf
#		./main 2022 6 1 3 36  1> /tmp/TEMPORARY/libradtran-interface.log
#	echo "64-25_1M_N10 finished"
#done

