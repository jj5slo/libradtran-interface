import numpy as np #numpy(ナンパイ): 数値計算
import scipy as sp #scipy(サイパイ): 数学・科学計算
import matplotlib.pyplot as plt #matplotlib: プロット
import pandas as pd #pandas(パンダス): データ解析
# import japanize_matplotlib # 日本語フォント表示のライブラリ 3.12では動かないかも？

# %precision 5 # 有効数字

photons = [300, 750, 3000, 4700, 18750, 75000, 300000]

ph = 300

data = pd.read_csv("./pho"+str(ph)+".dat", sep='\s+', skiprows=1, header=None, index_col=0)
print(data)
df = data.T
print(df)

my_bins = np.linspace(0.95, 1.05, 30)
print(df[0].value_counts(bins=my_bins, sort=None))
mean, std = pd.Series(df[0]).mean(), pd.Series(df[0]).std()

seriesdata_normalized = pd.Series(df[0]) / mean

pdf = sp.stats.norm.pdf(my_bins, 1.0, std / mean)

plt.hist(seriesdata_normalized, ec="black", lw=2, density=True, bins=my_bins)
# plt.rcParams['font.family'] = 'Hiragino Maru Gothic Pro'
plt.xlabel("Normalized radiance (Simulated)")
plt.ylabel("Probability density (Frequency)")
plt.title("photons = "+str(photons)+", Tangential height "+str(0)+" [km]")
plt.grid(True)
plt.plot(my_bins, pdf)
plt.show()

