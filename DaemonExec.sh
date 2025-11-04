#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log

cp configs/pho300000 config.conf
month=6
date=1
i=86

for j in `seq 1 100`; do
	echo "Progress: $month $date, $i, pho300000 $j th time"
	./main 2022 "$month" "$date" "$i" &> /tmp/TEMPORARY/libradtran-interface.log
done
# CPU time を調べるため手動でやる
# ただし、24時間でやらないと意味がない（状況でかかる時間でしかない）
# cp configs/pho300000 config.conf
# cp configs/pho75000 config.conf
# cp configs/pho18750 config.conf
# cp configs/pho4700 config.conf
# cp configs/pho3000 config.conf
# cp configs/pho750 config.conf
# cp configs/pho300 config.conf




# for month in 6; do
# # month=6
# 	for date in 1 21; do	
# #		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
# 		for i in 5 15 35 45 75 85; do
# 	 		echo "Progress: $month $date, $i"
# 			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
# 		done
# 	done
# done
# cp configs/pho300000 config.conf
# 
# for month in 6; do
# # month=6
# 	for date in 1 21; do	
# #		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
# 		for i in 5 15 35 45 75 85; do
# 	 		echo "Progress: $month $date, $i"
# 			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
# 		done
# 	done
# done
# cp configs/pho6000 config.conf
# 
# for month in 6; do
# # month=6
# 	for date in 1 21; do	
# #		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
# 		for i in 5 15 35 45 75 85; do
# 	 		echo "Progress: $month $date, $i"
# 			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
# 		done
# 	done
# done
# cp configs/pho60000 config.conf
# 
# for month in 6; do
# # month=6
# 	for date in 1 21; do	
# #		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
# 		for i in 5 15 35 45 75 85; do
# 	 		echo "Progress: $month $date, $i"
# 			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
# 		done
# 	done
# done
# cp configs/pho600000 config.conf
# 
# for month in 6; do
# # month=6
# 	for date in 1 21; do	
# #		for i in 5 10 15 20 25 30 35 40 45 50 65 70 75 80 85; do
# 		for i in 5 15 35 45 75 85; do
# 	 		echo "Progress: $month $date, $i"
# 			./main 2022 "$month" "$date" "$i" &> /tmp/libradtran-interface.log
# 		done
# 	done
# done
