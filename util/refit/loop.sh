#!/bin/bash


for file in ~/SANO/research/estimate-profile/Result/03-W1/2021-09-19_band3/*/*/*/*/result*_0.dat; do
	./refit "${file}" 30 64 "${file}_refit30to64.dat" 90
done
