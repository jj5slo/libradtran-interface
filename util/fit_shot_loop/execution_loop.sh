#!/usr/bin/env bash
set -euo pipefail
shopt -s nullglob

BASE="../../../2026/2026-10w4/plusminusDT/result"

REF="$BASE/afglus_log_101"
M10="$BASE/afglus_50_45_254.2K"
M5="$BASE/afglus_50_45_259.2K"
ZERO="$BASE/afglus_50_45_264.2K"
P5="$BASE/afglus_50_45_269.2K"
P10="$BASE/afglus_50_45_274.2K"

OUTPUT="result.dat"

# 最大の数値名ディレクトリからファイルを取得
get_latest_file() {
	local dir="$1"
	local latest
	local files

	latest=$(
		find "$dir" -mindepth 1 -maxdepth 1 -type d -printf '%f\n' |
		sort -n |
		tail -n 1
	)

	if [[ -z "$latest" ]]; then
		echo "No directory found: $dir" >&2
		return 1
	fi

	files=("$dir/$latest"/result_*.dat)

	if [[ ${#files[@]} -ne 1 ]]; then
		echo "Expected one result file: $dir/$latest" >&2
		return 1
	fi

	printf '%s\n' "${files[0]}"
}

# ヘッダー
echo "#SZA PHI -10K -5K 0K +5K +10K (RMSE)" > "$OUTPUT"

first_sza=true

# SZAのループ
while IFS= read -r sza_dir; do
	SZA=$(basename "$sza_dir")

	if [[ "$first_sza" == false ]]; then
		echo >> "$OUTPUT"
	fi
	first_sza=false

	# PHIのループ
	while IFS= read -r phi_dir; do
		PHI=$(basename "$phi_dir")

		ref_FILE=$(get_latest_file "$REF/$SZA/$PHI")
		FILE_m10=$(get_latest_file "$M10/$SZA/$PHI")
		FILE_m5=$(get_latest_file "$M5/$SZA/$PHI")
		FILE_0=$(get_latest_file "$ZERO/$SZA/$PHI")
		FILE_p5=$(get_latest_file "$P5/$SZA/$PHI")
		FILE_p10=$(get_latest_file "$P10/$SZA/$PHI")

		echo "Processing SZA=$SZA PHI=$PHI"

		./fit_shot_loop \
			"$ref_FILE" \
			"$SZA" "$PHI" \
			"$FILE_m10" \
			"$FILE_m5" \
			"$FILE_0" \
			"$FILE_p5" \
			"$FILE_p10" \
			"$OUTPUT"

	done < <(
		find "$sza_dir" -mindepth 1 -maxdepth 1 -type d | sort -V
	)

done < <(
	find "$REF" -mindepth 1 -maxdepth 1 -type d | sort -V
)

echo >> "$OUTPUT"

echo "Completed: $OUTPUT"
