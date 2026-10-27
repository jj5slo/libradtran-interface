#!/usr/bin/env bash
set -euo pipefail

BASE="../../../2026/2026-10w4/plusminusDT/RESULT/obsolete"

ref_FILE=(
	"$BASE"/afglus_log_101/0/10/*/result_*.dat
)

FILE_m10=(
	"$BASE"/afglus_50_45_254.2K/0/10/*/result_*.dat
)

FILE_m5=(
	"$BASE"/afglus_50_45_259.2K/0/10/*/result_*.dat
)

FILE_0=(
	"$BASE"/afglus_50_45_264.2K/0/10/*/result_*.dat
)

FILE_p5=(
	"$BASE"/afglus_50_45_269.2K/0/10/*/result_*.dat
)

FILE_p10=(
	"$BASE"/afglus_50_45_274.2K/0/10/*/result_*.dat
)

./fit_shot_loop \
	"${ref_FILE[@]}" \
	0 0 \
	"${FILE_m10[@]}" \
	"${FILE_m5[@]}" \
	"${FILE_0[@]}" \
	"${FILE_p5[@]}" \
	"${FILE_p10[@]}" \
	"test_output.dat"
