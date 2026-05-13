import os
import glob
import math

minheight=10
maxheight=89


def process_data(ldir, lat0, lon0):
	# カレントディレクトリ直下の 4桁の数字ディレクトリ（0000, 0010 ...）を取得
	# base_dirs = glob.glob(ldir+'/[0-9][0-9][0-9][0-9]')

	out_file = ldir+"/lerr_out"+str(minheight)+"to"+str(maxheight)+".dat"
	
	with open(out_file, 'w') as outfile:
		print(f"#lat lon hour sza_on_ground mse rmse log_square_error(raw)", file=outfile)
		for hour_i in range(0, 23, 1):
			for minute_i in range(0, 59, 30):
				hour = hour_i + (float(minute_i) / 60.0)
#			for bdir in sorted(base_dirs):
				bdir = ldir+f"/{hour_i:02}{minute_i:02}"
				if not os.path.isdir(bdir):
					print("not bdir")
					print(f"{lat0} {lon0} {hour} NaN NaN NaN NaN", file=outfile)
					continue
					
				# その下にある数字名のディレクトリをすべて取得
				subdirs = []
				for d in os.listdir(bdir):
					if d.isdigit() and os.path.isdir(os.path.join(bdir, d)):
						subdirs.append(int(d))
						
				if not subdirs:
					print("not subdirs")
					print(f"{lat0} {lon0} {hour} NaN NaN NaN NaN", file=outfile)
					continue
					
				# 数字が「最大の数」のディレクトリを特定
				max_subdir = str(max(subdirs))
				
				# ターゲットとなる result*.dat ファイルのパスを構築
				target_dir = os.path.join(bdir, max_subdir, 'run_once')
				dat_files = glob.glob(os.path.join(target_dir, 'result*refit'+str(minheight)+'to'+str(maxheight)+'.dat'))
				if not dat_files:
					print("not dat_files")
					print(f"{lat0} {lon0} {hour} NaN NaN NaN NaN", file=outfile)
					continue

				for dat_file in dat_files:
					try:
						print(dat_file)
						with open(dat_file, 'r') as f:
							lines = f.readlines()
							
#						# 行数が足りないファイルはスキップ
#						if len(lines) < 15:
#							continue
#							
#						# 7行目 (Pythonのインデックスは0始まりなので6) から sza_on_ground を抽出
#						line3 = lines[2]
#						line4 = lines[3]
#						line7 = lines[6]
#						line8 = lines[7]
#						line14 = lines[13]
#						if 'longitude' in line3 and 'latitude' in line4 and 'Hour=' in line7 and 'sza_on_ground:' in line8 and 'log_square_error' in line14:
#							# 'Hour=' で分割し、後ろ側([1])を取得。さらに '#' で分割し、前側([0])を取得して余白を消す
#		#					hour_str = line7.split('Hour=')[1].split('#')[0].strip()
#							longitude_str = line3.split('longitude:')[1].strip()
#							longitude = float(longitude_str)
#							latitude_str  = line4.split('latitude:')[1].strip()
#							latitude  = float(latitude_str)
#
#							hour_str = line7.split('Hour=')[1].strip()
#							hour = float(hour_str)
#							
#							# 'sza_on_ground:' で分割し、後ろ側([1])を取得して余白を消す
#							sza_str = line8.split('sza_on_ground:')[1].strip()
#							sza_on_ground = float(sza_str)
#		
#							log_square_error_str = line14.split('log_square_error:')[1].split('#')[0].strip()
#							log_square_error = float(log_square_error_str)
#						else:
#							continue
#		 
#						# 16行目 (インデックス15) 以降のデータを処理
						hour = None
						sza_on_ground = None
						log_square_error = None
						lrmse_refit = None
						latitude = None
						longitude = None
						sq_diff_sum = 0.0
						count = 0
						
						for line in lines:
							clean_line = line.strip()
							if not clean_line:# 空行をスキップ
								continue

							if clean_line.startswith('#'):
								if 'latitude:' in clean_line:
									latitude = float(clean_line.split('latitude:')[1].split('#')[0].strip())
								if 'longitude:' in clean_line:
									longitude = float(clean_line.split('longitude:')[1].split('#')[0].strip())
								if 'Hour=' in clean_line:
									hour = float(clean_line.split('Hour=')[1].split('#')[0].strip())
								if 'sza_on_ground:' in clean_line:
									sza_on_ground = float(clean_line.split('sza_on_ground:')[1].split('#')[0].strip())
								if 'log_square_error:' in clean_line:
									log_square_error = float(clean_line.split('log_square_error:')[1].split('#')[0].strip())
								if 'log_square_error ( index' in clean_line:
									lrmse_refit = float(clean_line.split(':')[1].split('#')[0].strip())
								continue

							cols = clean_line.split()
							# 少なくとも5列以上のデータがある行のみ処理
							# minheight から maxheight km までのみ処理
							if len(cols) >= 5 and minheight <= float(cols[0]) and float(cols[0]) <= maxheight:
								val2 = float(cols[1]) # 2列目
								val5 = float(cols[4]) # 5列目
								
								# log10の計算のため、両方が0より大きい場合のみ処理する
								if val2 > 0 and val5 > 0:
									# 差をとる前にそれぞれlog10を適用
									diff = math.log10(val2) - math.log10(val5)
									sq_diff_sum += diff ** 2
									count += 1
		 
						# 二乗平均を計算して出力
						if count > 0:
							mse = sq_diff_sum / count
							rmse = math.sqrt(mse)
							# ※もし二乗平均平方根 (RMSE) にしたい場合は math.sqrt(mse) としてください
							print(f"{latitude} {longitude} {hour} {sza_on_ground} {mse} {rmse} {lrmse_refit}", file=outfile)
						
					except Exception as e:
						# 読み込みエラーなどがあった場合は標準エラーに出力
						import sys
						print(f"# Error processing {dat_file} : {e}", file=sys.stderr)

if __name__ == '__main__':
	lat0s = [038.0, 039.0, 040.0, 041.0, 042.0, 043.0, 044.0, 045.0, 046.0, 047.0, 048.0, 049.0, 050.0, 051.0, 052.0, 053.0, 054.0, 055.0, 056.0, 057.0, 058.0, 059.0, 060.0, 061.0, 062.0, 063.0, 064.0, 065.0, 066.0, 067.0, 068.0, 069.0, 070.0, 071.0, 072.0, 073.0, 074.0, 075.0, 076.0, 077.0, 078.0, 079.0, 080.0, 081.0, 081.0, 080.0, 079.0, 078.0, 077.0, 076.0, 075.0, 074.0, 073.0, 072.0, 071.0, 070.0, 069.0, 068.0, 067.0, 066.0, 065.0, 064.0, 063.0, 062.0, 061.0, 060.0, 059.0, 058.0, 057.0, 056.0, 055.0, 054.0, 053.0, 052.0, 051.0, 050.0, 049.0, 048.0, 047.0, 046.0, 045.0, 044.0, 043.0, 042.0, 041.0, 040.0, 039.0, 038.0]
	lon0s = [061.8, 062.0, 062.2, 062.3, 062.5, 062.7, 062.9, 063.1, 063.4, 063.6, 063.8, 064.1, 064.4, 064.7, 065.0, 065.3, 065.7, 066.1, 066.5, 066.9, 067.4, 067.9, 068.4, 069.0, 069.6, 070.3, 071.0, 071.8, 072.7, 073.6, 074.6, 075.8, 077.1, 078.5, 080.2, 082.0, 084.2, 086.7, 089.7, 093.3, 097.8, 103.7, 112.3, 128.8, 152.6, 169.1, 177.7, 183.6, 188.1, 191.7, 194.7, 197.2, 199.4, 201.2, 202.9, 204.3, 205.6, 206.8, 207.8, 208.7, 209.6, 210.4, 211.1, 211.8, 212.4, 213.0, 213.5, 214.0, 214.5, 214.9, 215.3, 215.7, 216.1, 216.4, 216.7, 217.0, 217.3, 217.6, 217.8, 218.0, 218.3, 218.5, 218.7, 218.9, 219.1, 219.2, 219.4, 219.6]
	latlon_dirs = glob.glob('[0-9][0-9]')
	for ldir in sorted(latlon_dirs):
		process_data(ldir, lat0s[int(ldir)-1], lon0s[int(ldir)-1])
