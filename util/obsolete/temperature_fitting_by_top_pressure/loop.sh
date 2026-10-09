#!/bin/bash

# ../../Result/ 以下のすべてのファイルに対してループ処理
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N5/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N10/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N5/merged/museigen/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/55/N5/merged/*; do
for file in ~/SANO/research/estimate-profile/Result/02-W2/mod65_0.5/merged/*.dat; do
    # ファイルが実際に存在するか確認（ディレクトリが空の場合のエラー防止）
    if [ -f "$file" ]; then
        echo "実行中: ./prog \"$file\""
        ./temperature_fitting_by_top_pressure "../../../Result/01-W3/20190101_0750/aura_mls2019_01_01_0750.dat" "$file" 30 61
    fi
done
