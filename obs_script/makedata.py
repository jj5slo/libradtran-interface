### データをテキストに変換 ###
#
# root_dir (現在は . )の下の
#  (root_dir)/data/b01/s01s02/201607/20160719/
# にある pickle 形式のデータを、同じディレクトリ内のテキストファイルに変換
#
import pickle
import numpy as np
import sys

if len(sys.argv) != 8:
  print( len(sys.argv) )
  print('USAGE: python3 makedata.py [YEAR] [MONTH] [DAY] [HOUR] [MIN] [n(orth)/s(outh)] [1-3(BAND)]')
  print('e.g: python3 makedata.py 2016 7 9 21 00 n 1')
  sys.exit(0)

year, month, day, hour, minute, north_or_south, band = sys.argv[1:8]

str_hemisphere = 's01s02' if north_or_south == 'n' else 's09s10'
str_band = '0' + str(int(band))
str_month = month if int(month) >= 10 else '0'+str(int(month))
str_day   = day   if int(day)   >= 10 else '0'+str(int(day))
str_hour  = hour  if int(hour)  >= 10 else '0'+str(int(hour))
str_min   = '00'  if int(minute) == 0 else minute

# PKL, DATAディレクトリ
root_dir = '.'
str_data_dir = root_dir + '/data/' \
  + 'b' + str_band + '/' \
  + str_hemisphere + '/' \
  + year + str_month + '/' \
  + year + str_month + str_day + '/'

str_scan = '0110_S0210' if north_or_south == 'n' else '0910_S1010'
str_r = 'R05' if band == '3' else 'R10'

# PKL
str_pklfile = str_data_dir \
  + 'HS_H08_' + year + str_month + str_day + '_' + str_hour + str_min + '_' \
  + 'B' + str_band + '_FLDK_' + str_r + '_S' + str_scan + '_ave_1.0deg_1.0km.pkl'

# DATA
# 以下はカウント値の平均用
str_txtdata = str_data_dir \
  + 'h08_b' + str_band + '_' + str_hemisphere + '_' \
  + year + str_month + str_day + '_' + str_hour + str_min + '00.txt'
# 以下はカウント値の標準偏差用
#str_txtdata = str_data_dir \
#  + 'sigma_b' + str_band + '_' + str_hemisphere + '_' \
#  + year + str_month + str_day + '_' + str_hour + str_min + '00.txt'
print(str_txtdata)

try:
  with open(str_pklfile, 'rb') as f:
#with open('HS_H08_20220101_2350_B01_FLDK_R10_S0110_S0210_ave_1.0deg_1.0km.pkl', 'rb') as f:
    s = pickle.load(f)
except Exception as err:
  print(err)
  sys.exit(0)


### データ出力
#
str_output = ''
#
str_format = '{:0=5.1f}'
#
# 1行目は "            height"
str_output += '            height \n'
#
# 2行目は "lat   lon   000.0 001.0 ... 100.0"
str_output += 'lat   lon  '
for i in range(101):
  str_output += ' ' + str_format.format(i)
str_output += ' \n'
#
# 3行目以降は "(経度) (緯度) (カウント値) (カウント値) ... (カウント値)"
#
str_output_east = '' # 東側
WEST_SIDE = 0
EAST_SIDE = 1
for lat in range(44):
  str_output      += str_format.format( s[2][lat] )
  str_output_east += str_format.format( s[2][43-lat] )
  str_output      += ' ' + str_format.format( s[7][WEST_SIDE][lat] )
  str_output_east += ' ' + str_format.format( s[7][EAST_SIDE][43-lat] )
  for alt in range(101):
    # 以下はカウント値の平均用
    str_output      += ' ' + str_format.format( s[0][WEST_SIDE][lat][alt] )
    str_output_east += ' ' + str_format.format( s[0][EAST_SIDE][43-lat][alt] )
    # 以下はカウント値の標準偏差用
    # str_output      += ' ' + str_format.format( s[1][WEST_SIDE][lat][alt] )
    # str_output_east += ' ' + str_format.format( s[1][EAST_SIDE][43-lat][alt] )
  str_output      += ' \n'
  str_output_east += ' \n'

str_output += str_output_east

with open(str_txtdata, 'w') as f:
  f.write( str_output )
