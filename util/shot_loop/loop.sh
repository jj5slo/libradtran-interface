#!/bin/bash
shopt -s nullglob
mkdir -p "/home/sano/sano1/temporary1/shot_loop/PARA1_"

#for SZA in {45..60..15}; do
for SZA in 65 70 80 75; do
  # SZAに応じてPHIのステップ幅（度）を変更
  if [ "$SZA" -eq 0 ]; then
    PHI_STEP=185     # 天頂（0度）では方位角依存が無いため 0 のみ
  elif [ "$SZA" -le 30 ]; then
    PHI_STEP=30      # 低SZA: 30度刻み
  elif [ "$SZA" -le 60 ]; then
    PHI_STEP=20      # 中SZA: 20度刻み
  else
    PHI_STEP=5      # 高SZA: 5度刻み
  fi

  # 動的なステップ幅に対応した for ループ
  for (( PHI=0; PHI<=59; PHI+=PHI_STEP )); do
		echo "${SZA} ${PHI}"
    for atm_file in /home/sano/sano1/research1/estimate-profile1/2026/2026-10w5/plusminusDT/atm/afglus_*.dat; do
      atmname=$(basename "$atm_file" .dat)
			DIR_RESULT="/home/sano/sano1/research1/estimate-profile1/2026/2026-10w5/plusminusDT/${atmname}/${SZA}/${PHI}/"
			mkdir -p ${DIR_RESULT}
      ./shot_loop "$SZA" "$PHI" "$atm_file" "${DIR_RESULT}" "PARA1_" >> /home/sano/sano1/research1/estimate-profile1/2026/2026-10w5/plusminusDT/log_PARA1.log
    done
  done
done

