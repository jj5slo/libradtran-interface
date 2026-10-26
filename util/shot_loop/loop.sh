#!/bin/bash
shopt -s nullglob

for SZA in {0..90..5}; do
  # SZAに応じてPHIのステップ幅（度）を変更
  if [ "$SZA" -eq 0 ]; then
    PHI_STEP=180     # 天頂（0度）では方位角依存が無いため 0 と 180 のみ
  elif [ "$SZA" -le 30 ]; then
    PHI_STEP=15      # 低SZA: 15度刻み
  elif [ "$SZA" -le 60 ]; then
    PHI_STEP=10      # 中SZA: 10度刻み
  else
    PHI_STEP=5       # 高SZA: 5度刻み
  fi

  # 動的なステップ幅に対応した for ループ
  for (( PHI=0; PHI<=180; PHI+=PHI_STEP )); do
    for atm_file in ../atmosphere_edit/afglus_*.dat; do
      atmname=$(basename "$atm_file" .dat)
      ./shot_loop "$SZA" "$PHI" "$atm_file" "RESULT/${atmname}/${SZA}/${PHI}/"
    done
  done
done

