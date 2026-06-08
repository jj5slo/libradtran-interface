#!/bin/bash


#for file in ~/SANO/research/estimate-profile/Result/03-W1/2021-09-19_band3/*/*/*/*/result*_0.dat; do
#for file in ~/SANO/research/estimate-profile/2026/2026-04W4/2021-09-05_rad_b_ratio_shot/shot_raw.dat; do
for file in ~/SANO/research/estimate-profile/2026/2026-06w1/2021-09-05_b_0_crs_shot_b/07/0430/1780510292/run_once/re*_0.dat; do
	./refit "${file}" 25 90 "${file}_refit25to90.dat" 22.0
done
