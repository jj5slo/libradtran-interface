#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

mv configs/pho3000 config.conf
for month in 6; do
# month=6
#	for date in 1 21; do	
	for date in 21; do
#		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
		for i in 5 15 35 45 75 85; do
	 		echo "Progress: $month $date, $i"
			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
		done
	done
done

mv configs/pho30000 config.conf

for month in 6; do
# month=6
	for date in 1 21; do	
#		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
		for i in 5 15 35 45 75 85; do
	 		echo "Progress: $month $date, $i"
			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
		done
	done
done
mv configs/pho300000 config.conf

for month in 6; do
# month=6
	for date in 1 21; do	
#		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
		for i in 5 15 35 45 75 85; do
	 		echo "Progress: $month $date, $i"
			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
		done
	done
done
mv configs/pho6000 config.conf

for month in 6; do
# month=6
	for date in 1 21; do	
#		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
		for i in 5 15 35 45 75 85; do
	 		echo "Progress: $month $date, $i"
			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
		done
	done
done
mv configs/pho60000 config.conf

for month in 6; do
# month=6
	for date in 1 21; do	
#		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
		for i in 5 15 35 45 75 85; do
	 		echo "Progress: $month $date, $i"
			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
		done
	done
done
mv configs/pho600000 config.conf

for month in 6; do
# month=6
	for date in 1 21; do	
#		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
		for i in 5 15 35 45 75 85; do
	 		echo "Progress: $month $date, $i"
			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
		done
	done
done
