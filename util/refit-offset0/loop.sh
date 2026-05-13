#!/bin/bash


#for file in ~/SANO/research/estimate-profile/Result/03-W1/2021-09-19_band3/*/*/*/*/result*_0.dat; do
for file in ~/SANO/research/estimate-profile/2026/2026-04W4/2021-09-05_rad_b_ratio_shot/result*sub*.dat; do
	./refit "${file}" 30 60 "${file}_refit30to60.dat"
done
