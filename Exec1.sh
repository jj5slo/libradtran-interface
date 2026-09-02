#! /bin/bash

# echo "started"
pwd
#cd /home/sano/sano1/research1/estimate-profile1/result1/libradtran-interface

cp config.conf __config.conf

cp shot1.conf config.conf
for year in 2015 2016 2017 2018 2019; do
	for month in 9; do
		for day in 5; do
			for hour in $(seq 0 1 23); do
				for minute in 0; do
					#for lineno in `seq 1 1 88`; do
					for lineno in $(seq 1 2 88); do
						for backgroundintensity in "0"; do
							yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
							hourminute=$(printf "%02d%02d" "$hour" "$minute")
							linenumber=$(printf "%02d" "$lineno")
								mkdir -p "/home/sano/sano1/research1/estimate-profile1/result1/2026/2026-06w1/${yeardate}_b_${backgroundintensity}_albedo09_1Mp/${linenumber}/${hourminute}"
								sed -i "11s|.*|DIR_RESULT=/home/sano/sano1/research1/estimate-profile1/result1/2026/2026-06w1/${yeardate}_b_${backgroundintensity}_albedo09_1Mp/${linenumber}/${hourminute}/|" config.conf
								echo   "11th line replaced."
								sed -i "16s|.*|MOLECULES_FILE=/home/sano/sano1/research1/estimate-profile1/result1/2026/2026-06w1/afglus_101_log.dat|" config.conf
								echo   "16th line replaced."
								mkdir -p "/home/sano/sano1/research1/estimate-profile1/result1/2026/2026-06w1/${yeardate}_b_${backgroundintensity}_albedo09_1Mp/${linenumber}/${hourminute}"
								sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
								echo   "17th line replaced."
								sed -i "18s|.*|PATH_ATMOSPHERE_INIT=/home/sano/sano1/research1/estimate-profile1/result1/2026/2026-06w1/${yeardate}_b_${backgroundintensity}_albedo09_1Mp/${linenumber}/${hourminute}/atminit.dat|" config.conf
								echo   "18th line replaced."
								sed -i "32s|.*|OBS_BACKGROUND_INTENSITY=${backgroundintensity}|" config.conf
								echo   "32th line replaced. ${backgroundintensity}."
								sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nverbose\\nmol_abs_param crs\\n|' config.conf
								echo   "36th line replaced.(mol_abs_param crs)"
								sed -i "37s|.*|SURFACE_TYPE=LAMBERT|" config.conf
								echo   "37th line replaced(LAMBERT)."
								sed -i "38s|.*|albedo=0.9|" config.conf
								echo   "38th line replaced(albedo=0.9)."
								sed -i "43s|.*|mc_photons=1000000|" config.conf
								echo   "43th line replaced(1M photons)."
							echo "started $yeardate $hourminute $linenumber"
								./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /home/sano/sano1/temporary1/libradtran-interface.log
							cp /home/sano/sano1/temporary1/libradtran-interface.log /home/sano/sano1/research1/estimate-profile1/result1/2026/2026-06w1/${yeardate}_b_${backgroundintensity}_albedo09_1Mp/${linenumber}/${hourminute}/libradtran-interface.log
							echo "finished"
						done
					done
				done
			done
		done
	done
done


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
