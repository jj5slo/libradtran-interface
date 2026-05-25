#!/bin/bash

# ../../Result/ 以下のすべてのファイルに対してループ処理
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N5/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N10/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N5/merged/museigen/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/60/N10/merged/*; do
#for file in ~/SANO/research/estimate-profile/Result/01-W5/20190101_0750/90/N5/temp/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/02-W2/MSIS/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/02-W2/mod65_2/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/03-W1/2021-09-19_ret/0750-27/atm/*atm*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/03-W1/2021-09-23_ret/0900-07/atm/*atm*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/03-W1/2021-09-10_ret/87/2240/temp/*atm*.dat; do
#for file in ~/SANO/research/estimate-profile/2026/2026-05w1/merged_2M/*atm*[0-9].dat; do
for file in ~/SANO/research/estimate-profile/2026/2026-05w4/merged_brdf_cam/absorb/*/*.dat; do
    # ファイルが実際に存在するか確認（ディレクトリが空の場合のエラー防止）
    if [ -f "$file" ]; then
        echo "実行中: ./prog \"$file\""
        ./main_atmNair_to_temperature "$file" 60 1
    fi
done
