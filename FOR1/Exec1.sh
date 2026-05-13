#! /bin/bash

# echo "started"
pwd
#cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface

cp config.conf __config.conf

cp CONFIGS/ret.conf config.conf
year=2021
month=9
day=19
hour=13
minute=00
lineno=1
yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
hourminute=$(printf "%02d%02d" "$hour" "$minute")
linenumber=$(printf "%02d" "$lineno")
  mkdir -p "/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_ret/${linenumber}/atm"
	mkdir -p "/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_ret/${linenumber}/${hourminute}"
	sed -i "11s|.*|DIR_RESULT=/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_ret/${linenumber}/${hourminute}/|" config.conf
	echo "11th line replaced."
	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_ret/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
	echo "17th line replaced."
echo "started $yeardate $hourminute $linenumber"
	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /home/sano/sano1/temporary1/libradtran-interface.log
echo "finished"


#cp CONFIGS/1_band1_loop.conf config.conf
#yeardate="2021-09-19"
#for lineno in `paste -d '\n' <(seq 1 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band2_aerosol/${linenumber}/atm"
#	for hour in `seq 0 23`; do
#		for minute in `seq 0 30 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band2_aerosol/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band2_aerosol/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band2_aerosol/${linenumber}/atm/atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2021 09 19 "${hour}" "${minute}" "${linenumber}"  1> /home/sano/sano1/temporary1/libradtran-interface.log
#		done
#	done
#done
#

#cp CONFIGS/band1_loop.conf config.conf
#year=2021
#month=9
#date=19
#yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$date")
#for lineno in `paste -d '\n' <(seq 1 4 44) <(seq 88 -4 45)`; do
#  linenumber=$(printf "%02d" "$lineno")
#  mkdir -p "/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band1_aerosol_visibility_100/${linenumber}/atm"
#  for hour in `seq 0 23`; do
#    for minute in `seq 0 30 59`; do
#      hourminute=$(printf "%02d%02d" "$hour" "$minute")
#      echo "$linenumber $hourminute"
#      mkdir -p "/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band1_aerosol_visibility_100/${linenumber}/${hourminute}"
#      sed -i "11s|.*|DIR_RESULT=/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band1_aerosol_visibility_100/${linenumber}/${hourminute}/|" config.conf
#      echo "11th line replaced."
#      sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/home/sano/sano1/research1/estimate-profile1/results1/${yeardate}_band1_aerosol_visibility_100/${linenumber}/atm/${yeardate}atm${hourminute}.dat|" config.conf
#      echo "17th line replaced."
#      ./main "${year}" "${month}" "${date}" "${hour}" "${minute}" "${linenumber}"  1> /home/sano/sano1/temporary1/libradtran-interface.log
#    done
#  done
#done
