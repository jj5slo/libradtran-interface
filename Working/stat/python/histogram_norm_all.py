import numpy as np #numpy(ナンパイ): 数値計算
import scipy as sp #scipy(サイパイ): 数学・科学計算
import matplotlib.pyplot as plt #matplotlib: プロット
import pandas as pd #pandas(パンダス): データ解析
import japanize_matplotlib # 日本語フォント表示のライブラリ 3.12では動かないかも？

import math
import os

# %precision 5 # 有効数字

photons = [300, 750, 3000, 4700, 18750, 75000, 300000]


plt.grid(True)
plt.xlabel("Radiance / 平均値")
plt.ylabel("確率密度 (頻度)")

for line_index in range(0,101):
	print("=============="+str(line_index))
	plt.grid(True)
	plt.xlabel("Radiance / 平均値")
	plt.ylabel("確率密度 (頻度)")
	means = []
	sds = []
	output_dir = "./lines/"+str(line_index)+"/"
	if not os.path.isdir(output_dir):
		os.mkdir(output_dir)
	output_suffix = "_norm_jp.png"
	output_stat_suffix = "_stat.txt"
	
		
	for ph in photons:
		output_filename = output_dir+str(line_index)+"_pho"+str(ph)+output_suffix
		data = pd.read_csv("./pho"+str(ph)+".dat", sep='\s+', skiprows=1, header=None, index_col=0)
#		print(data)
		df = data.T
#		print(df)
		
		my_bins = np.linspace(0.95, 1.05, int(math.sqrt(50* ph/300)))
		pdf_bins = np.linspace(0.95, 1.05, int(math.sqrt(3000 * ph/300)))
#		print(df[line_index].value_counts(bins=my_bins, sort=None))
		seriesdata = pd.Series(df[line_index])
		mean, std = seriesdata.mean(), seriesdata.std()
		means.append(mean)
		sds.append(std)
		
		seriesdata_normalized = pd.Series(df[line_index]) / mean
		
		pdf = sp.stats.norm.pdf(pdf_bins, 1.0, std / mean)
		
		plt.hist(seriesdata_normalized, ec="black", lw=2, density=True, bins=my_bins)#, alpha=0.4)
	#	plt.hist(seriesdata, ec="black", lw=2, density=True, bins=my_bins)#, alpha=0.4)
		# plt.rcParams['font.family'] = 'Hiragino Maru Gothic Pro'
	#	plt.xlabel("Normalized radiance (Simulated)")
	#	plt.ylabel("Probability density (Frequency)")
		plt.title("photons = "+str(ph)+", Tangential height "+str(line_index)+" [km]")
		plt.plot(pdf_bins, pdf)
		plt.savefig(output_filename, format="png", dpi=600)
	
	plt.title("Tangential height "+str(line_index)+" [km]")
	print(means)
	print(sds)
	sds_per_means = []
	for sd_i, mean_i in zip(sds, means):
		sds_per_means.append(sd_i/mean_i)
		
	print(sds_per_means)
	
	statistics = pd.DataFrame([means, sds, sds_per_means], index = ["mean", "standard_deviation", "sd/mean"], columns = photons)
	with open(output_dir+str(line_index)+output_stat_suffix, mode="w") as f:
		f.write("# Tangential_height: "+str(line_index)+"\n")
		statistics.T.to_csv(f, index=True, header=True, sep=" ")
	
	# plt.yscale('log')
	# plt.ylim([0, 200])
	plt.savefig(output_dir+str(line_index)+"_all"+output_suffix, format="png", dpi=600)
#	plt.show()
	plt.clf()
	plt.close()
		
