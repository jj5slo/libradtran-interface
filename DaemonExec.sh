#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

cp config.conf __config.conf

cp CONFIGS/2019-01-01_loop.conf config.conf
yeardate="2019-01-01"
for lineno in 25; do
	linenumber=$(printf "%02d" "$lineno")
	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm"
	for hour in `seq 0 23`; do
		for minute in `seq 0 10 59`; do
			hourminute=$(printf "%02d%02d" "$hour" "$minute")
			echo "$linenumber $hourminute"
			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}"
			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}/|" config.conf
			echo "11th line replaced."
			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
			echo "17th line replaced."
			./main 2019 01 01 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
		done
	done
done

cp CONFIGS/2022-06-03_loop.conf config.conf
yeardate="2022-06-03"
for lineno in 2; do
	linenumber=$(printf "%02d" "$lineno")
	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm"
	for hour in `seq 7 23`; do
		for minute in `seq 0 10 59`; do
			hourminute=$(printf "%02d%02d" "$hour" "$minute")
			echo "$linenumber $hourminute"
			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}"
			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}/|" config.conf
			echo "11th line replaced."
			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
			echo "17th line replaced."
			./main 2022 06 03 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
		done
	done
done

for lineno in `paste -d '\n' <(seq 6 4 44) <(seq 88 -4 45)`; do
	linenumber=$(printf "%02d" "$lineno")
	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm"
	for hour in `seq 0 23`; do
		for minute in `seq 0 10 59`; do
			hourminute=$(printf "%02d%02d" "$hour" "$minute")
			echo "$linenumber $hourminute"
			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}"
			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}/|" config.conf
			echo "11th line replaced."
			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
			echo "17th line replaced."
			./main 2022 06 03 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
		done
	done
done

#cp CONFIGS/20190101_0750_MSIS.conf config.conf
#	./main 2019 01 01 07 50 25 1> /tmp/TEMPORARY/libradtran-interface.log

#cp CONFIGS/opt_to_MSIS_N5.conf config.conf
#for i in `seq 1 3`; do
#	echo "$i / 3 times repeating"
#	
#	echo "started"
#		./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#	echo "finished"
#
#done

#cp CONFIGS/plainparallel.conf config.conf
#	echo "started plainparallel"
#	./main 2022 06 01 03 00 36  1> /tmp/TEMPORARY/libradtran-interface.log

#cp CONFIGS/spherical.conf config.conf
#	echo "started spherical"
#	./main 2022 06 01 03 00 36  1> /tmp/TEMPORARY/libradtran-interface.log
#

#cp CONFIGS/20190101_0750_shot.conf config.conf
#	echo "started spectrum"
#	./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log


#for i in `seq 1 2`; do
#	echo "$i / 2 times repeating"
#	cp CONFIGS/20190101_0750_65up_0.5.conf config.conf
#		echo "started 0.5"
#			./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#		echo "finished"
	#cp CONFIGS/20190101_0750_65up_0.7.conf config.conf
	#	echo "started 0.7"
	#		./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
	#	echo "finished"
	#cp CONFIGS/20190101_0750_65up_0.8.conf config.conf
	#	echo "started"
	#		./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
	#	echo "finished"
#	cp CONFIGS/20190101_0750_N5.conf config.conf
#		echo "started"
#			./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#		echo "finished"
#	cp CONFIGS/20190101_0750_65up_1.2.conf config.conf
#		echo "started"
#			./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#		echo "finished"
#	cp CONFIGS/20190101_0750_65up_1.5.conf config.conf
#		echo "started"
#			./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#		echo "finished"
#	cp CONFIGS/20190101_0750_65up_2.conf config.conf
#		echo "started"
#			./main 2019 01 01 07 50 25  1> /tmp/TEMPORARY/libradtran-interface.log
#		echo "finished"
#done
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

