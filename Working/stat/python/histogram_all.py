import numpy as np #numpy(ナンパイ): 数値計算
import scipy as sp #scipy(サイパイ): 数学・科学計算
import matplotlib.pyplot as plt #matplotlib: プロット
import pandas as pd #pandas(パンダス): データ解析
import japanize_matplotlib # 日本語フォント表示のライブラリ 3.12では動かないかも？

import math

# %precision 5 # 有効数字

photons = [300, 750, 3000, 4700, 18750, 75000, 300000]

means = []
sds = []

plt.grid(True)
plt.xlabel("Radiance")
plt.ylabel("確率密度 (頻度)")

line_index = 1

output_suffix = "_jp.png"

for ph in photons:
	output_filename = "./lines/"+str(line_index)+"/"+str(line_index)+"_pho"+str(ph)+output_suffix
	data = pd.read_csv("./pho"+str(ph)+".dat", sep='\s+', skiprows=1, header=None, index_col=0)
	print(data)
	df = data.T
	print(df)
	
	my_bins = np.linspace(0.128, 0.137, int(math.sqrt(50* ph/300)))
	pdf_bins = np.linspace(0.128, 0.137, int(math.sqrt(3000 * ph/300)))
	print(df[line_index].value_counts(bins=my_bins, sort=None))
	seriesdata = pd.Series(df[line_index])
	mean, std = seriesdata.mean(), seriesdata.std()
	means.append(mean)
	sds.append(std)
	
#	seriesdata_normalized = pd.Series(df[line_index]) / mean
	
	pdf = sp.stats.norm.pdf(pdf_bins, mean, std)# / mean)
	
#	plt.hist(seriesdata_normalized, ec="black", lw=2, density=True, bins=my_bins)#, alpha=0.4)
	plt.hist(seriesdata, ec="black", lw=2, density=True, bins=my_bins)#, alpha=0.4)
	# plt.rcParams['font.family'] = 'Hiragino Maru Gothic Pro'
#	plt.xlabel("Normalized radiance (Simulated)")
#	plt.ylabel("Probability density (Frequency)")
	plt.title("photons = "+str(photons)+", Tangential height "+str(line_index)+" [km]")
	plt.plot(pdf_bins, pdf)
	plt.savefig(output_filename, format="png", dpi=600)

plt.title("Tangential height "+str(line_index)+" [km]")
print(means)
print(sds)
# plt.yscale('log')
# plt.ylim([0, 200])
plt.savefig("./lines/"+str(line_index)+"/"+str(line_index)+"_all"+output_suffix, format="png", dpi=600)
plt.show()
	
