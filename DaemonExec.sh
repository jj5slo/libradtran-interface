#! /bin/bash

# echo "started"
pwd
cd /home/sano/SANO/research/estimate-profile/libradtran-interface
# ./main 2022 6 2 3
for i in $(seq 3 90); do
	echo "Progress: $i in (3 to 90)"
  ./main 2022 6 30 "$i"
done
 


