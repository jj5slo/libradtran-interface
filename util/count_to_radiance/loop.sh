#!/bin/bash



# 空の状態から書き込むためにリセット
> in_files.txt
> out_files.txt

for file in ~/DATA/testdata/*b01*0910*.txt; do
    # a/ 内のファイルのフルパスを取得
    abs_path_a=$(readlink -f "$file")
    echo "$abs_path_a" >> in_files.txt
    
    # b/ 用のファイル名を作成
    filename=$(basename "$file")
    # b/ ディレクトリ自体のフルパスを取得して結合
    # (b ディレクトリが既に存在している必要があります)
    abs_path_b="/lhome/sano2/DATA/testdata_rad/${filename%.*}.txt"
    echo "$abs_path_b" >> out_files.txt
done

