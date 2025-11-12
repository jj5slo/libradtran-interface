#! /bin/bash

for j in `seq 1 30`; do
	for line in `seq 0 87`; do
		./core 2022 6 "$j" "$line"
	done
done
