#!/bin/bash

# ../../Result/ 以下のすべてのファイルに対してループ処理
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N5/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N10/merged/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/N5/merged/museigen/*.dat; do
#for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/60/N10/merged/*; do
#for file in ~/SANO/research/estimate-profile/Result/01-W5/20190101_0750/90/N5/temp/*.dat; do
for file in ~/SANO/research/estimate-profile/Result/01-W3/20190101_0750/opt_to_MSIS/merged/*.dat; do
    # ファイルが実際に存在するか確認（ディレクトリが空の場合のエラー防止）
    if [ -f "$file" ]; then
        echo "実行中: ./prog \"$file\""
        ./main_atmNair_to_temperature "$file" 65 1
    fi
done
