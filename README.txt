libradtran-interface
last update: 2025/07/14


## mainでする予定のこと
	北極を0度として、ぐるっと円周何度方向で、高度何キロ方向を見たいのかを決める。
	それをある時刻で緯度経度に変換する。（この緯度経度がデータファイルにあるかどうかを確認する）
	その緯度経度、時刻での太陽天頂角、方位角を計算する（これは、大気上層でも同じ方向という近似をする）（sza, phi0）
	視線方向とTOAの交点のうち衛星に近い方での、視線方向の天頂角（対地表）及び方位角を計算する。（umu(cosに直していれる), phi）
	
	zout TOA およびsza, phi0, umu, phi からuvspecの標準入力に渡すファイルを用意する
	
	大気プロファイルを決めてファイルに書き込む。
	mc*.radを消す(大気プロファイルの数が未定の場合)
	
	uvspecを実行する
	mc*.radから放射輝度('escape' radiance)を読み出す
	観測データと引き算
	最適化プログラムに渡して次の大気プロファイルを決めさせる
	（ループ）

	
	
# 各ファイルの解説
## ヘッダ
	interface.h
	coordinate.h
	solar_direction.h
	FixedParams.h
	execute.h
	layered_atmosphere.h

ファイル作成系(interface.h)
	interface-stdin.cpp	libRadtranの入力ファイルを作る
	interface-atmosphere.cpp	libRadtranの大気プロファイル設定ファイルを作る

座標変換系(coordinate.h, solar_direction.h)
	translate-coordinate.cpp
		一般的な座標変換

	太陽方向とかをPMCのプログラムで計算して、それを地表から見た天頂角とかに変換する。
	LookingDirection.cpp
	Geocoordinate.cpp
		改修流用
	across_point_atmosphere.cpp
		改修流用
	cross_point_at_altitude.cpp
		改修流用
	solar_direction.cpp
		流用
		経緯度から太陽天頂角・方位角を求める
	sensor_direction.cpp
		ある点を衛星からみたときの視線方向の（衛星から点向きの）水平からの角度と方位角を出す


地球・衛星軌道の情報クラス(FixedParams.h)
	FixedParams.h
		PlanetParam 半径など
		SatelliteParam 半径など
	
	
大気クラス
	layered-atmosphere.h



"Paramなんとか"は入出力、"なんとかParam"は諸量とする
