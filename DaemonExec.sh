#! /bin/bash

# echo "started"
pwd
cd /home/sano/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log
# for month in 6 9; do
month=6
	for date in 1 30; do
		for i in 2 22 42 65 85; do
	 		echo "Progress: $month $date, $i in 2 22 42 65 85"
			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
		done
	done
# done
 


