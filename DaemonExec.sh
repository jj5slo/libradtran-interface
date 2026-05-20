#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

cp config.conf __config.conf

#cp CONFIGS/shot.conf config.conf
#for lineno in `paste -d '\n' <(seq 1 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	for hour in `seq 0 1 23`; do
#		for minute in 20 40; do
#		   year=2021
#		   month=9
#		   day=5
#		#   hour=9
#		#   minute=10
#		#   lineno=1
#		   yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#		   hourminute=$(printf "%02d%02d" "$hour" "$minute")
#		   linenumber=$(printf "%02d" "$lineno")
#		  # 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
#		   	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W4/${yeardate}_shotforratio60-55/${linenumber}/${hourminute}"
#		   	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W4/${yeardate}_shotforratio60-55/${linenumber}/${hourminute}/|" config.conf
#		   	echo "11th line replaced."
#		   	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W4/afglus_101_log.dat|" config.conf
#		   	echo "16th line replaced."
#		   	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W4/${yeardate}_shotforratio60-55/${linenumber}/${hourminute}"
#		   	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W4/${yeardate}_shotforratio60-55/${linenumber}/${hourminute}/${yeardate}msis${hourminute}.dat|" config.conf
#		   	echo "18th line replaced."
#		   echo "started $yeardate $hourminute $linenumber"
#		   	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
#		   echo "finished"
#		done
#	done
#done
  


cp CONFIGS/shot.conf config.conf
#for backgroundintensity in "21.8" "21.9" "22.0" "22.1" "22.2" "22.3" "22.4"; do
#for backgroundintensity in "21.8"  "22.1" "22.3"; do
for backgroundintensity in "22.1"; do
#for backgroundintensity in "22.1"; do
 year=2021
 month=9
 day=5
 hour=9
 minute=10
 lineno=1
 yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
 hourminute=$(printf "%02d%02d" "$hour" "$minute")
 linenumber=$(printf "%02d" "$lineno")
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-05w3/${yeardate}_b_${backgroundintensity}_albedo0.9_shot/${linenumber}/${hourminute}"
 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-05w3/${yeardate}_b_${backgroundintensity}_albedo0.9_shot/${linenumber}/${hourminute}/|" config.conf
 	echo   "11th line replaced."
 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-05w3/afglus_101_log.dat|" config.conf
 	echo   "16th line replaced."
 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-05w3/${yeardate}_b_${backgroundintensity}_albedo0.9_shot/${linenumber}/${hourminute}"
 	sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
 	echo   "17th line replaced."
 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-05w3/${yeardate}_b_${backgroundintensity}_albedo0.9_shot/${linenumber}/${hourminute}/atminit.dat|" config.conf
 	echo   "18th line replaced."
 	sed -i "32s|.*|OBS_BACKGROUND_INTENSITY=${backgroundintensity}|" config.conf
 	echo   "32th line replaced. ${backgroundintensity}."
 	sed -i "37s|.*|SURFACE_TYPE=LAMBERT|" config.conf
 	echo   "37th line replaced."
 	sed -i "38s|.*|albedo=0.9|" config.conf
 	echo   "38th line replaced."
 echo "started $yeardate $hourminute $linenumber"
 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
 echo "finished"
done

#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
# 	echo "17th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#
#year=2021
# month=9
# day=10
# hour=22
# minute=40
# lineno=87
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_single/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
# 	echo "17th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#
#cp CONFIGS/band1_aerosol_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
# 	echo "17th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#
#year=2021
# month=9
# day=10
# hour=22
# minute=40
# lineno=87
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W2/${yeardate}_ret_aerosol_single/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
# 	echo "17th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#


#cp CONFIGS/band1_loop_NO2modified.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
## 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
## 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#
#cp CONFIGS/band1_loop_NO2modified.conf config.conf
# year=2021
# month=9
# day=10
# hour=22
# minute=40
# lineno=87
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
## 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglusNO2modified/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
## 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"


#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_noMOL/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_noMOL/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_noMOL.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_noMOL.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#
#
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_1e9/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_1e9/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_1e9.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_1e9.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_2e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_2e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_2e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_2e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_3e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_3e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_3e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_3e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_4e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_4e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_4e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_4e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_5e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_5e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_5e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_5e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_6e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_6e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_6e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_6e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_7e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_7e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_7e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_7e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_8e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_8e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_8e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_8e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"
#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_9e10/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_9e10/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_9e10.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_9e10.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"

#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=5
# hour=9
# minute=10
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_1e13/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus_NO2_40-60_1e13/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "16s|.*|MOLECULES_FILE=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_1e13.dat|" config.conf
# 	echo "16th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/testNO2distribution/modify_no2/afglus_NO2_40-60_1e13.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"



#cp CONFIGS/band1_loop.conf config.conf
# year=2021
# month=9
# day=10
# hour=22
# minute=40
# lineno=87
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
## 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
## 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_single_afglus/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
## 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"



#cp CONFIGS/ret.conf config.conf
# year=2021
# month=9
# day=19
# hour=13
# minute=0
# lineno=1
# yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
# hourminute=$(printf "%02d%02d" "$hour" "$minute")
# linenumber=$(printf "%02d" "$lineno")
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_ret_MolAerosolRad/${linenumber}/atm"
# 	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_ret_MolAerosolRad/${linenumber}/${hourminute}"
# 	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_ret_MolAerosolRad/${linenumber}/${hourminute}/|" config.conf
# 	echo "11th line replaced."
# 	sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_ret_MolAerosolRad/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
# 	echo "18th line replaced."
# echo "started $yeardate $hourminute $linenumber"
# 	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
# echo "finished"


##cp CONFIGS/ret_band2.conf config.conf
#year=2021
#month=9
#day=26
#hour=22
#minute=40
#lineno=84
#yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#hourminute=$(printf "%02d%02d" "$hour" "$minute")
#linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/atm"
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/${hourminute}"
#	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/${hourminute}/|" config.conf
#	echo "11th line replaced."
#	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
#	echo "17th line replaced."
#echo "started $yeardate $hourminute $linenumber"
#	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
#echo "finished"

#cp CONFIGS/ret_band3.conf config.conf
#year=2021
#month=9
#day=26
#hour=22
#minute=40
#lineno=84
#yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#hourminute=$(printf "%02d%02d" "$hour" "$minute")
#linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band3/${linenumber}/atm"
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band3/${linenumber}/${hourminute}"
#	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band3/${linenumber}/${hourminute}/|" config.conf
#	echo "11th line replaced."
#	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band3/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
#	echo "17th line replaced."
#echo "started $yeardate $hourminute $linenumber"
#	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
#echo "finished"

#cp CONFIGS/single_band2.conf config.conf
#year=2021
#month=9
#day=26
#hour=22
#minute=40
#lineno=84
#yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#hourminute=$(printf "%02d%02d" "$hour" "$minute")
#linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/atm"
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/${hourminute}"
#	sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/${hourminute}/|" config.conf
#	echo "11th line replaced."
#	sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_ret_band2/${linenumber}/atm/${yeardate}msis${hourminute}.dat|" config.conf
#	echo "17th line replaced."
#echo "started $yeardate $hourminute $linenumber"
#	./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libradtran-interface.log
#echo "finished"



#cp CONFIGS/band1_loop.conf config.conf
#year=2021
#month=9
#date=19
#yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$date")
#for lineno in `paste -d '\n' <(seq 1 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_MOL_NairMinus/${linenumber}/atm"
#	for hour in `seq 0 3 23`; do
#		for minute in `seq 0 60 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_MOL_NairMinus/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_MOL_NairMinus/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_MOL_NairMinus/${linenumber}/atm/${yeardate}atm${hourminute}.dat|" config.conf
#			echo "18th line replaced."
#			./main "${year}" "${month}" "${date}" "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done

#cp CONFIGS/band1_loop_no2only.conf config.conf
#year=2021
#month=9
#date=19
#yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$date")
#for lineno in `paste -d '\n' <(seq 1 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_NO2_/${linenumber}/atm"
#	for hour in `seq 0 23`; do
#		for minute in `seq 0 30 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_NO2_/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_NO2_/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/2026/2026-04W3/${yeardate}_band1_NO2_/${linenumber}/atm/${yeardate}atm${hourminute}.dat|" config.conf
#			echo "18th line replaced."
#			./main "${year}" "${month}" "${date}" "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done









#cp CONFIGS/band2_aerosol_loop.conf config.conf
#yeardate="2021-09-19"
#for lineno in `paste -d '\n' <(seq 1 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band2_aerosol/${linenumber}/atm"
#	for hour in `seq 0 23`; do
#		for minute in `seq 0 30 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band2_aerosol/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band2_aerosol/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band2_aerosol/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2021 09 19 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done
#
#cp CONFIGS/band3_aerosol_loop.conf config.conf
#yeardate="2021-09-19"
#for lineno in `paste -d '\n' <(seq 1 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band3_aerosol/${linenumber}/atm"
#	for hour in `seq 0 23`; do
#		for minute in `seq 0 30 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band3_aerosol/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band3_aerosol/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}_band3_aerosol/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2021 09 19 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done

#cp CONFIGS/2019-01-01_loop.conf config.conf
#yeardate="2021-09-23"
#for lineno in `paste -d '\n' <(seq 6 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm"
#	for hour in `seq 0 23`; do
#		for minute in `seq 0 10 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2021 09 23 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done

#cp CONFIGS/2022-06-03_loop.conf config.conf
#yeardate="2022-06-03"
#for lineno in 42; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/atm"
#	for hour in 18; do
#		for minute in `seq 50 10 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2022 06 03 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#	for hour in `seq 19 23`; do
#		for minute in `seq 0 10 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2022 06 03 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done
#for lineno in `seq 52 -4 45`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/atm"
#	for hour in `seq 0 23`; do
#		for minute in `seq 0 10 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/new_format/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2022 06 03 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done

#for lineno in `paste -d '\n' <(seq 6 4 44) <(seq 88 -4 45)`; do
#	linenumber=$(printf "%02d" "$lineno")
#	mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm"
#	for hour in `seq 0 23`; do
#		for minute in `seq 0 10 59`; do
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#			echo "$linenumber $hourminute"
#			mkdir -p "/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}"
#			sed -i "11s|.*|DIR_RESULT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/${hourminute}/|" config.conf
#			echo "11th line replaced."
#			sed -i "17s|.*|PATH_ATMOSPHERE_INIT=/lhome/sano2/SANO/research/estimate-profile/Result/03-W1/${yeardate}/${linenumber}/atm/220603atm${hourminute}.dat|" config.conf
#			echo "17th line replaced."
#			./main 2022 06 03 "${hour}" "${minute}" "${linenumber}"  1> /tmp/TEMPORARY/libradtran-interface.log
#		done
#	done
#done

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

