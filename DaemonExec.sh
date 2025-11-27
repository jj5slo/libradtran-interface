#! /bin/bash

# echo "started"
pwd
cd /lhome/sano2/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
# ./main 2022 6 6 6 &> /tmp/libradtran-interface.log


#cp configs/black.conf config.conf
#for i in `seq 1 10`;do
#	echo "Progress: $month $date, $i,"
	./main 2022 6 1 3 36 1> /tmp/TEMPORARY/libradtran-interface.log
#done



# for month in 6 ;do
# 	for date in 1 ;do
# 		for i in 56 76 86; do
# 			echo "Progress: $month $date, $i,"
# 			./main 2022 "$month" "$date" "$i" &> /tmp/TEMPORARY/libradtran-interface.log
# 		done
# 	done
# done
# for month in 6 9;do
# 	for date in 11 21 30;do
# 		for i in 6 16 26 36 46 56 76 86; do
# 			echo "Progress: $month $date, $i,"
# 			./main 2022 "$month" "$date" "$i" &> /tmp/TEMPORARY/libradtran-interface.log
# 		done
# 	done
# done
# 
# month=6
# date=1
# i=86
# 
# for j in `seq 1 200`; do
# 	echo "Progress: $month $date, $i, pho100000 $j th time"
# 	./main 2022 "$month" "$date" "$i" &> /tmp/TEMPORARY/libradtran-interface.log
# done
# # CPU time を調べるため手動でやる
# # ただし、24時間でやらないと意味がない（状況でかかる時間でしかない）
# 
# 
# cp configs/pho3000000 config.conf
# for j in `seq 1 70`; do
# 	echo "Progress: $month $date, $i, pho3000000 $j th time"
# 	./main 2022 "$month" "$date" "$i" &> /tmp/TEMPORARY/libradtran-interface.log
# done
# cp configs/pho1000000 config.conf
# for j in `seq 1 100`; do
# 	echo "Progress: $month $date, $i, pho1000000 $j th time"
# 	./main 2022 "$month" "$date" "$i" &> /tmp/TEMPORARY/libradtran-interface.log
# done
# 
# cp configs/pho47000 config.conf
# for j in `seq 1 500`; do
# 	echo "Progress: $month $date, $i, pho47000 $j th time"
# 	./main 2022 "$month" "$date" "$i" &> /tmp/TEMPORARY/libradtran-interface.log
# done
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
