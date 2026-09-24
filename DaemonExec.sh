#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libr0/libradtran-interface || exit 1
mkdir -p /tmp/TEMPORARY/libr0

# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

cp config.conf __config.conf


#for lineno in `paste -d '\n' <(seq 9 1 44) <(seq 88 -1 45)`; do

for lineno in 76; do
	for aurafile in /lhome/sano2/DATA/auradata/$(printf "%02d" ${lineno})/*/*/aura*.dat; do
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
			if [ "$month" -ge 5 ]; then
				continue
#			elif [ "$month" -le 6 ]; then
#				continue
			fi


			echo "target: ${year}-${month}-${day} ${hour}:${minute}"
			orig_yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")

			# ---- bの計算 ----
			cp CONFIGS/shot.conf config.conf
			linenumber=$(printf "%02d" "$lineno")
			yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
			hourminute=$(printf "%02d%02d" "$hour" "$minute")

			DIR_RESULT="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/shot_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}"
			DIR_MSIS="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/msis/${linenumber}/${yeardate}/${hourminute}"
			
			shopt -s nullglob
			exist_files=("${DIR_RESULT}"/*/*/result*$(printf "%04d%02d%02d" ${year} ${month} ${day})*$(printf "%02d%02d00" ${hour} ${minute})*.dat)
			shopt -u nullglob
			if [ ${#exist_files[@]} -gt 0 ]; then
				echo "Skipping shot for ${year}-${month}-${day} (result*.dat already exists)."
				continue
			fi
	
			mkdir -p "${DIR_RESULT}"
			mkdir -p "${DIR_MSIS}"
			sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" config.conf
			sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
			sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" config.conf
			sed -i "25s|.*|i_top=70|" config.conf
			sed -i "26s|.*|i_bottom=15|" config.conf
			sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/b_0.dat|" config.conf
			sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' config.conf
			sed -i "37s|.*|SURFACE_TYPE=ABSORB|" config.conf
			sed -i "43s|.*|mc_photons=100000000|" config.conf
			./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libr0/libradtran-interface.log
			cp /tmp/TEMPORARY/libr0/libradtran-interface.log "${DIR_RESULT}/shot_libradtran-interface.log"
			
			for shot_file in /lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/shot_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}/*/*/result*.dat; do
				./util/calc_b/calc_b ${shot_file}
				mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/b_55-59/${linenumber}/${orig_yeardate}/${hourminute}"
				./util/pickup_b/pickup_b 55 59 "${shot_file}_b.dat" -o "/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/b_55-59/${linenumber}/${orig_yeardate}/${hourminute}/b_55-59_${orig_yeardate}_${hourminute}_${linenumber}.dat"
				mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/b_50-54/${linenumber}/${orig_yeardate}/${hourminute}"
				./util/pickup_b/pickup_b 50 54 "${shot_file}_b.dat" -o "/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/b_50-54/${linenumber}/${orig_yeardate}/${hourminute}/b_50-54_${orig_yeardate}_${hourminute}_${linenumber}.dat"
			done

			cp CONFIGS/ret.conf config.conf
			linenumber=$(printf "%02d" "$lineno")
			yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
			hourminute=$(printf "%02d%02d" "$hour" "$minute")

			DIR_RESULT="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/ret_b50-54_std0.5e-2/${linenumber}/${yeardate}/${hourminute}"
			DIR_MSIS="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/msis/${linenumber}/${yeardate}/${hourminute}"

			mkdir -p "${DIR_RESULT}"
			sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" config.conf
			mkdir -p "${DIR_MSIS}"
			sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
			sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" config.conf
			sed -i "25s|.*|i_top=49|" config.conf
			sed -i "26s|.*|i_bottom=40|" config.conf
			sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/b_50-54/${linenumber}/${orig_yeardate}/${hourminute}/b_50-54_${orig_yeardate}_${hourminute}_${linenumber}.dat|" config.conf
			sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' config.conf
			sed -i "37s|.*|SURFACE_TYPE=ABSORB|" config.conf
			sed -i "43s|.*|mc_photons=100000000|" config.conf
			sed -i "48s|.*|OPTIMIZER=BO|" config.conf
			echo   "48th line replaced(OPTIMIZER=BO)."
			sed -i "49s|.*|BO_N_ITER=60|" config.conf
			echo   "49th (BO_N_ITER=60)."
			sed -i "51s|.*|XTOL=1.0e-6|" config.conf
			echo "started $yeardate $hourminute $linenumber"
			./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libr0/libradtran-interface.log
			cp /tmp/TEMPORARY/libr0/libradtran-interface.log "${DIR_RESULT}/libradtran-interface.log"
			echo "finished(b_50-54)"
			
			cp CONFIGS/ret.conf config.conf
			linenumber=$(printf "%02d" "$lineno")
			yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
			hourminute=$(printf "%02d%02d" "$hour" "$minute")

			DIR_RESULT="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/ret_b55-59_std0.5e-2/${linenumber}/${yeardate}/${hourminute}"
			DIR_MSIS="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/msis/${linenumber}/${yeardate}/${hourminute}"

			mkdir -p "${DIR_RESULT}"
			sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" config.conf
			mkdir -p "${DIR_MSIS}"
			sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
			sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" config.conf
			sed -i "25s|.*|i_top=49|" config.conf
			sed -i "26s|.*|i_bottom=40|" config.conf
			sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/const_b/b_55-59/${linenumber}/${orig_yeardate}/${hourminute}/b_55-59_${orig_yeardate}_${hourminute}_${linenumber}.dat|" config.conf
			sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' config.conf
			sed -i "37s|.*|SURFACE_TYPE=ABSORB|" config.conf
			sed -i "43s|.*|mc_photons=100000000|" config.conf
			sed -i "48s|.*|OPTIMIZER=BO|" config.conf
			echo   "48th line replaced(OPTIMIZER=BO)."
			sed -i "49s|.*|BO_N_ITER=60|" config.conf
			echo   "49th (BO_N_ITER=60)."
			sed -i "51s|.*|XTOL=1.0e-6|" config.conf
			echo "started $yeardate $hourminute $linenumber"
			./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libr0/libradtran-interface.log
			cp /tmp/TEMPORARY/libr0/libradtran-interface.log "${DIR_RESULT}/libradtran-interface.log"
			echo "finished(b_55-59)"
		fi

	done
done






#for lineno in 76; do
#	for aurafile in /lhome/sano2/DATA/auradata/$(printf "%02d" ${lineno})/*/*/aura*.dat; do
#		aurafilename=$(basename "$aurafile")
#		if [[ $aurafilename =~ aura[-_]([0-9]{4})-([0-9]{2})-([0-9]{2})_([0-9]{2})([0-9]{2})_ ]]; then
#			YYYY="${BASH_REMATCH[1]}"
#			MM="${BASH_REMATCH[2]}"
#			DD="${BASH_REMATCH[3]}"
#			hh="${BASH_REMATCH[4]}"
#			mm="${BASH_REMATCH[5]}"
#	
#			# 【プロの四捨五入ロジック】
#			read -r r_year r_month r_day r_hour r_min <<< "$(date -d "${YYYY}-${MM}-${DD} ${hh}:${mm}:00 5 minutes" +"%Y %m %d %H %M" 2>/dev/null)"
#			# 日付パースに失敗した場合はスキップ
#			if [ -z "$r_year" ]; then
#				echo "Warning: Failed to parse date from $aurafilename"
#				continue
#			fi
#			# 5分足した状態の「分」の一の位を 0 に書き換える（＝10分単位への切り捨て）
#			# これにより、結果的に元の時刻の「四捨五入」が完了する
#			rounded_min="${r_min:0:1}0"
#	
#			# 4. 「10#」を付与して10進数として評価することで、安全に0埋めを除去
#			# (これを行わないと、08や09が8進数エラーになります)
#			year=$((10#$r_year))
#			month=$((10#$r_month))
#			day=$((10#$r_day))
#			hour=$((10#$r_hour))
#			minute=$((10#$rounded_min))
#			if [ "$month" -ge 5 ]; then
#				continue
##			elif [ "$month" -le 6 ]; then
##				continue
#			fi
#
#
#			echo "target: ${year}-${month}-${day} ${hour}:${minute}"
#			orig_yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#
#			offsets=("2 day ago:2da" "1 day ago:1da" "1 day hence:1dh" "2 day hence:2dh")
#			for item in "${offsets[@]}"; do
#				offset="${item%:*}"
#				suffix="${item#*:}"
#			
#				read -r temp_year temp_month temp_day <<< "$(date -d "${year}-${month}-${day} ${offset}" +"%-Y %-m %-d")"
#				echo "${temp_year}-${temp_month}-${temp_day} ${hour}:${minute}"
#				cp CONFIGS/shot.conf config.conf
#				linenumber=$(printf "%02d" "$lineno")
#				yeardate=$(printf "%04d-%02d-%02d" "$temp_year" "$temp_month" "$temp_day")
#				hourminute=$(printf "%02d%02d" "$hour" "$minute")
#
#				DIR_RESULT="/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/shot_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}"
#				DIR_MSIS="/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/msis/${linenumber}/${yeardate}/${hourminute}"
#				
#				shopt -s nullglob
#				exist_files=("${DIR_RESULT}"/*/*/result*$(printf "%04d%02d%02d" ${temp_year} ${temp_month} ${temp_day})*$(printf "%02d%02d00" ${hour} ${minute})*.dat)
#				shopt -u nullglob
#				if [ ${#exist_files[@]} -gt 0 ]; then
#					echo "Skipping shot for ${temp_year}-${temp_month}-${temp_day} (result*.dat already exists)."
#					continue
#				fi
#	
#				mkdir -p "${DIR_RESULT}"
#				mkdir -p "${DIR_MSIS}"
#				sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" config.conf
#				sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
#				sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" config.conf
#				sed -i "25s|.*|i_top=70|" config.conf
#				sed -i "26s|.*|i_bottom=15|" config.conf
#				sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/b_0.dat|" config.conf
#				sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' config.conf
#				sed -i "37s|.*|SURFACE_TYPE=ABSORB|" config.conf
#				sed -i "43s|.*|mc_photons=100000000|" config.conf
#				./main "$temp_year" "$temp_month" "$temp_day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libr0/libradtran-interface.log
#				cp /tmp/TEMPORARY/libr0/libradtran-interface.log "${DIR_RESULT}/${suffix}_libradtran-interface.log"
#			done
#			
#			all_shots_exist=true
#			for item in "${offsets[@]}"; do
#				offset="${item%:*}"
#				# 確認する日付を再計算
#				read -r chk_year chk_month chk_day <<< "$(date -d "${year}-${month}-${day} ${offset}" +"%-Y %-m %-d")"
#				
#				DIR_RESULT_CHK="/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/shot_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}"
#				
#				shopt -s nullglob
#				chk_files=("${DIR_RESULT_CHK}"/*/*/result*$(printf "%04d%02d%02d" ${chk_year} ${chk_month} ${chk_day})*$(printf "%02d%02d00" ${hour} ${minute})*.dat)
#				shopt -u nullglob
#
#				# ファイルが見つからなかった場合
#				if [ ${#chk_files[@]} -eq 0 ]; then
#					echo "Missing shot data for ${chk_year}-${chk_month}-${chk_day}. Skipping make_b and ret."
#					all_shots_exist=false
#					break # 1つでも欠けていたらループを抜ける
#				fi
#			done
#
#			# 4日分揃っていなければ、現在の aurafile の処理を中断し、次の aurafile へ進む
#			if [ "$all_shots_exist" = false ]; then
#				continue
#			fi
#			echo "All 4 shot files exist. Proceeding to make_b and ret..."
#
#			exist_b_file="/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/b_4days/${linenumber}/${orig_yeardate}/${hourminute}/meaned_b_${orig_yeardate}_${hourminute}_${linenumber}_4days.dat_b.dat"
#			
#			# ファイルが「存在しない（! -f）」場合に中身を実行する
#			if [ ! -f "$exist_b_file" ]; then
#				for shot_file in /lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/shot_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}/*/*/result*.dat; do
#					./util/calc_b/calc_b ${shot_file}
#				done
#				mkdir -p "/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/b_4days/${linenumber}/${orig_yeardate}/${hourminute}"
#				./util/mean/mean /lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/shot_std0.5e-2/${linenumber}/${orig_yeardate}/${hourminute}/*/*/result*_b.dat -o /lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/b_4days/${linenumber}/${orig_yeardate}/${hourminute}/meaned_b_${orig_yeardate}_${hourminute}_${linenumber}_4days.dat
#				./util/make_b/make_b /lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/b_4days/${linenumber}/${orig_yeardate}/${hourminute}/meaned_b_${orig_yeardate}_${hourminute}_${linenumber}_4days.dat
#			else
#				echo "b file exests. Skipping make_b ..."
#			fi
#
#			cp CONFIGS/ret.conf config.conf
#			linenumber=$(printf "%02d" "$lineno")
#			yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#
#			DIR_RESULT="/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/ret_std0.5e-2/${linenumber}/${yeardate}/${hourminute}"
#			DIR_MSIS="/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/msis/${linenumber}/${yeardate}/${hourminute}"
#
#			mkdir -p "${DIR_RESULT}"
#			sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" config.conf
#			mkdir -p "${DIR_MSIS}"
#			sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
#			sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" config.conf
#			sed -i "25s|.*|i_top=49|" config.conf
#			sed -i "26s|.*|i_bottom=40|" config.conf
#			sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/b_4days/${linenumber}/${orig_yeardate}/${hourminute}/meaned_b_${orig_yeardate}_${hourminute}_${linenumber}_4days.dat_b.dat|" config.conf
#			sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' config.conf
#			sed -i "37s|.*|SURFACE_TYPE=ABSORB|" config.conf
#			sed -i "43s|.*|mc_photons=100000000|" config.conf
#			sed -i "48s|.*|OPTIMIZER=BO|" config.conf
#			echo   "48th line replaced(OPTIMIZER=BO)."
#			sed -i "49s|.*|BO_N_ITER=60|" config.conf
#			echo   "49th (BO_N_ITER=60)."
#			sed -i "51s|.*|XTOL=1.0e-6|" config.conf
#			echo "started $yeardate $hourminute $linenumber"
#			./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libr0/libradtran-interface.log
#			cp /tmp/TEMPORARY/libr0/libradtran-interface.log "${DIR_RESULT}/libradtran-interface.log"
#			echo "finished"
#		fi
#	done
#done
#



#for lineno in 16; do
#	for aurafile in /lhome/sano2/DATA/auradata/$(printf "%02d" ${lineno})/*/*/aura*.dat; do
#		aurafilename=$(basename "$aurafile")
#		if [[ $aurafilename =~ aura[-_]([0-9]{4})-([0-9]{2})-([0-9]{2})_([0-9]{2})([0-9]{2})_ ]]; then
#			YYYY="${BASH_REMATCH[1]}"
#			MM="${BASH_REMATCH[2]}"
#			DD="${BASH_REMATCH[3]}"
#			hh="${BASH_REMATCH[4]}"
#			mm="${BASH_REMATCH[5]}"
#	
#			# 【プロの四捨五入ロジック】
#			read -r r_year r_month r_day r_hour r_min <<< "$(date -d "${YYYY}-${MM}-${DD} ${hh}:${mm}:00 5 minutes" +"%Y %m %d %H %M" 2>/dev/null)"
#			# 日付パースに失敗した場合はスキップ
#			if [ -z "$r_year" ]; then
#				echo "Warning: Failed to parse date from $aurafilename"
#				continue
#			fi
#			# 5分足した状態の「分」の一の位を 0 に書き換える（＝10分単位への切り捨て）
#			# これにより、結果的に元の時刻の「四捨五入」が完了する
#			rounded_min="${r_min:0:1}0"
#	
#			# 4. 「10#」を付与して10進数として評価することで、安全に0埋めを除去
#			# (これを行わないと、08や09が8進数エラーになります)
#			year=$((10#$r_year))
#			month=$((10#$r_month))
#			day=$((10#$r_day))
#			hour=$((10#$r_hour))
#			minute=$((10#$rounded_min))
#			if [ "$month" -ge 3 ]; then
#				continue
##			elif [ "$month" -le 6 ]; then
##				continue
#			fi
#
#
#			echo "target: ${year}-${month}-${day} ${hour}:${minute}"
#			orig_yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#
#			DIR_B_CHK="/lhome/sano2/SANO/research/estimate-profile/2026/2026-07w1/b_4days/linearized"
#
#
#			exist_b_file="${DIR_B_CHK}/linear_b_${orig_yeardate}_$(printf "%02d%02d" ${hour} ${minute})_$(printf "%02d" "${lineno}").dat"
#			
#			# ファイルが見つからなかった場合
#			if [ ! -f "${exist_b_file}" ]; then
#				echo "Missing b data for ${orig_yeardate}. Skipping."
#				continue
#			fi
#			echo "Linearized b file exist. Proceeding to make_b and ret..."
#			
#			cp CONFIGS/ret.conf config.conf
#			linenumber=$(printf "%02d" "$lineno")
#			yeardate=$(printf "%04d-%02d-%02d" "$year" "$month" "$day")
#			hourminute=$(printf "%02d%02d" "$hour" "$minute")
#
#			DIR_RESULT="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/linear_ret_std0.5e-2/${linenumber}/${yeardate}/${hourminute}"
#			DIR_MSIS="/lhome/sano2/SANO/research/estimate-profile/2026/2026-09w1/msis/${linenumber}/${yeardate}/${hourminute}"
#
#			mkdir -p "${DIR_RESULT}"
#			sed -i "11s|.*|DIR_RESULT=${DIR_RESULT}/|" config.conf
#			mkdir -p "${DIR_MSIS}"
#			sed -i "17s|.*|FLAG_USE_ATMOSPHERE_INIT=0|" config.conf
#			sed -i "18s|.*|PATH_ATMOSPHERE_INIT=${DIR_MSIS}/msis_${yeardate}_${hourminute}_${linenumber}.dat|" config.conf
#			sed -i "25s|.*|i_top=49|" config.conf
#			sed -i "26s|.*|i_bottom=40|" config.conf
#			sed -i "32s|.*|PATH_OBS_BACKGROUND_INTENSITY=${exist_b_file}|" config.conf
#			sed -i '36s|.*|additional_option=aerosol_default\\nmc_vroom on\\nmc_std 0.5e-2\\nverbose\\nmol_abs_param crs\\n|' config.conf
#			sed -i "37s|.*|SURFACE_TYPE=ABSORB|" config.conf
#			sed -i "43s|.*|mc_photons=100000000|" config.conf
#			sed -i "48s|.*|OPTIMIZER=BO|" config.conf
#			echo   "48th line replaced(OPTIMIZER=BO)."
#			sed -i "49s|.*|BO_N_ITER=60|" config.conf
#			echo   "49th (BO_N_ITER=60)."
#			sed -i "51s|.*|XTOL=1.0e-6|" config.conf
#			echo "started $yeardate $hourminute $linenumber"
#			./main "$year" "$month" "$day" "$hour" "$minute" "$lineno"  1> /tmp/TEMPORARY/libr0/libradtran-interface.log
#			cp /tmp/TEMPORARY/libr0/libradtran-interface.log "${DIR_RESULT}/libradtran-interface.log"
#			echo "finished"
#		fi
#	done
#done
