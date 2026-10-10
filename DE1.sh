#! /bin/bash

# echo "started"
pwd
cd /home/sano/sano1/research1/estimate-profile1/2libradtran-interface/libradtran-interface
mkdir /home/sano/sano1/temporary1/2libr1

# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

cp 2config1.conf __2config1.conf


#for lineno in `paste -d '\n' <(seq 9 1 44) <(seq 88 -1 45)`; do

for lineno in 7; do
	for aurafile in /home/sano/sano1/data1/auradata/$(printf "%02d" ${lineno})/*/*/aura*.dat; do
		aurafilename=$(basename "$aurafile")
		if [[ $aurafilename =~ aura[-_]([0-9]{4})-([0-9]{2})-([0-9]{2})_([0-9]{2})([0-9]{2})_ ]]; then
			YYYY="${BASH_REMATCH[1]}"
			MM="${BASH_REMATCH[2]}"
			DD="${BASH_REMATCH[3]}"
			hh="${BASH_REMATCH[4]}"
			mm="${BASH_REMATCH[5]}"

			# 【プロの四捨五入ロジック】
			read -r r_year r_month r_day r_hour r_min <<< "$(date -d "${YYYY}-${MM}-${DD} ${hh}:${mm}:00 5 minutes" +"%Y %m %d %H %M" 2>/dev/null)"
			# 日付パースに失敗した場合はスキップ
			if [ -z "$r_year" ]; then
				echo "Warning: Failed to parse date from $aurafilename"
				continue
			fi
			# 5分足した状態の「分」の一の位を 0 に書き換える（＝10分単位への切り捨て）
			# これにより、結果的に元の時刻の「四捨五入」が完了する
			rounded_min="${r_min:0:1}0"

			# 4. 「10#」を付与して10進数として評価することで、安全に0埋めを除去
			# (これを行わないと、08や09が8進数エラーになります)
			year=$((10#$r_year))
			month=$((10#$r_month))
			day=$((10#$r_day))
			hour=$((10#$r_hour))
			minute=$((10#$rounded_min))
			if [ "$month" -le 3 ] || [ "$month" -ge 7 ]; then
				continue
#			elif [ "$month" -le 6 ]; then
#				continue
			fi


			echo "target: ${year}-${month}-${day} ${hour}:${minute}"
			orig_yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")

			# ---- bの計算 ----
			cp CONFIGS/2shot1.conf 2config1.conf
			linenumber=$(printf "%02d" "$lineno")
			yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
			hourminute=$(printf "%02d%02d" "$hour" "$minute")

			DIR_RESULT="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/shot_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}"
			DIR_MSIS="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/msis/${linenumber}/${yeardate}/${hourminute}"

			shopt -s nullglob
			flag_shot=0
			exist_files=("${DIR_RESULT}"/*/*/result*$(printf "%04d%02d%02d" ${year} ${month} ${day})*$(printf "%02d%02d00" ${hour} ${minute})*[0-9].dat)
			shopt -u nullglob
			if [ ${#exist_files[@]} -gt 0 ]; then
				echo "Skipping shot for ${year}-${month}-${day} (SHOTDIR/result*.dat already exists)."
				flag_shot=1
			fi

			if [ ${flag_shot} -eq 0 ]; then
				mkdir -p "${DIR_RESULT}"
				mkdir -p "${DIR_MSIS}"
				sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" 2config1.conf
				sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" 2config1.conf
				sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" 2config1.conf
				sed -i "25s|.*|i_top=70|" 2config1.conf
				sed -i "26s|.*|i_bottom=15|" 2config1.conf
				sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/b_0.dat|" 2config1.conf
				sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' 2config1.conf
				sed -i "37s|.*|SURFACE_TYPE=ABSORB|" 2config1.conf
				sed -i '39s|.*|mc_basename=mc2libr1|' 2config1.conf
				sed -i "43s|.*|mc_photons=100000000|" 2config1.conf
				./main "$year" "$month" "$day" "$hour" "$minute" "$lineno" "2config1.conf" 1> /home/sano/sano1/temporary1/2libr1/libradtran-interface.log
				cp /home/sano/sano1/temporary1/2libr1/libradtran-interface.log "${DIR_RESULT}/libradtran-interface.log"
			fi
			
			b5559dir="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/b_55-59/${linenumber}/${orig_yeardate}/${hourminute}"
			b5559file="${b5559dir}/b_55-59_${orig_yeardate}_${hourminute}_${linenumber}.dat"
			mkdir -p "${b5559dir}"
			b5054dir="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/b_50-54/${linenumber}/${orig_yeardate}/${hourminute}"
			b5054file="${b5054dir}/b_50-54_${orig_yeardate}_${hourminute}_${linenumber}.dat"
			mkdir -p "${b5054dir}"
			
			if [ -f ${DIR_RESULT}/*/*/result*[0-9].dat ]; then
			shot_file=(${DIR_RESULT}/*/*/result*[0-9].dat)
				./util/calc_b/calc_b ${shot_file}
				./util/pickup_b/pickup_b 55 59 "${shot_file}_b.dat" -o "${b5559file}"
				./util/pickup_b/pickup_b 50 54 "${shot_file}_b.dat" -o "${b5054file}"
			
				if [ ! -f "${b5054file}" ]; then
					echo "Warning: ${b5054file} was not generated. Skipping ret phase."
					continue
				fi
				if [ ! -f "${b5559file}" ]; then
					echo "Warning: ${b5559file} was not generated. Skipping ret phase."
					continue
				fi

				cp CONFIGS/2ret1.conf 2config1.conf
				linenumber=$(printf "%02d" "$lineno")
				yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
				hourminute=$(printf "%02d%02d" "$hour" "$minute")

				DIR_RESULT="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/ret_b50-54_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}"
				DIR_MSIS="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/msis/${linenumber}/${yeardate}/${hourminute}"
				
				shopt -s nullglob
				exist_files=("${DIR_RESULT}"/*/*/result*$(printf "%04d%02d%02d" ${year} ${month} ${day})*$(printf "%02d%02d00" ${hour} ${minute})*.dat)
				shopt -u nullglob
				if [ ${#exist_files[@]} -gt 0 ]; then
					echo "Skipping shot for ${year}-${month}-${day} (RETDIR/result*.dat already exists)."
					continue
				fi

				mkdir -p "${DIR_RESULT}"
				sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" 2config1.conf
				mkdir -p "${DIR_MSIS}"
				sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" 2config1.conf
				sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" 2config1.conf
				sed -i "25s|.*|i_top=49|" 2config1.conf
				sed -i "26s|.*|i_bottom=40|" 2config1.conf
				sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=${b5054file}|" 2config1.conf
				sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' 2config1.conf
				sed -i "37s|.*|SURFACE_TYPE=ABSORB|" 2config1.conf
				sed -i '39s|.*|mc_basename=mc2libr1|' 2config1.conf
				sed -i "43s|.*|mc_photons=100000000|" 2config1.conf
				sed -i "48s|.*|OPTIMIZER=BO|" 2config1.conf
				echo   "48th line replaced(OPTIMIZER=BO)."
				sed -i "49s|.*|BO_N_ITER=60|" 2config1.conf
				echo   "49th (BO_N_ITER=60)."
				sed -i "51s|.*|XTOL=1.0e-6|" 2config1.conf
				echo "started $yeardate $hourminute $linenumber"
				./main "$year" "$month" "$day" "$hour" "$minute" "$lineno" "2config1.conf" 1> /home/sano/sano1/temporary1/2libr1/libradtran-interface.log
				cp /home/sano/sano1/temporary1/2libr1/libradtran-interface.log "${DIR_RESULT}/libradtran-interface.log"
				echo "finished(b_50-54)"

				cp CONFIGS/2ret1.conf 2config1.conf
				linenumber=$(printf "%02d" "$lineno")
				yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
				hourminute=$(printf "%02d%02d" "$hour" "$minute")

				DIR_RESULT="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/ret_b55-59_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}"
				DIR_MSIS="/home/sano/sano1/research1/estimate-profile1/2026/2026-09w1/const_b/msis/${linenumber}/${yeardate}/${hourminute}"

				mkdir -p "${DIR_RESULT}"
				sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" 2config1.conf
				mkdir -p "${DIR_MSIS}"
				sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" 2config1.conf
				sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" 2config1.conf
				sed -i "25s|.*|i_top=49|" 2config1.conf
				sed -i "26s|.*|i_bottom=40|" 2config1.conf
				sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=${b5559file}|" 2config1.conf
				sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' 2config1.conf
				sed -i "37s|.*|SURFACE_TYPE=ABSORB|" 2config1.conf
				sed -i '39s|.*|mc_basename=mc2libr1|' 2config1.conf
				sed -i "43s|.*|mc_photons=100000000|" 2config1.conf
				sed -i "48s|.*|OPTIMIZER=BO|" 2config1.conf
				echo   "48th line replaced(OPTIMIZER=BO)."
				sed -i "49s|.*|BO_N_ITER=60|" 2config1.conf
				echo   "49th (BO_N_ITER=60)."
				sed -i "51s|.*|XTOL=1.0e-6|" 2config1.conf
				echo "started $yeardate $hourminute $linenumber"
				./main "$year" "$month" "$day" "$hour" "$minute" "$lineno" "2config1.conf" 1> /home/sano/sano1/temporary1/2libr1/libradtran-interface.log
				cp /home/sano/sano1/temporary1/2libr1/libradtran-interface.log "${DIR_RESULT}/libradtran-interface.log"
				echo "finished(b_55-59)"
			fi
		fi
	done
done

