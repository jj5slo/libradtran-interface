#include "interface.h"

#include <iostream>
#include <string>
#include <map>
#include <optional> // C++17以降

// 変換用関数の定義
// 文字列をenum SurfaceTypeに変換する関数
std::optional<SurfaceType> string_to_SurfaceType(const std::string& str) {
	// 文字列とenum値を対応させるマップ
	static const std::map<std::string, SurfaceType> stMap = {
		{"ABSORB", SurfaceType::ABSORB},
		{"LAMBERT", SurfaceType::LAMBERT},
		{"BLUE", SurfaceType::BLUE}
	};

	// マップを検索
	if (auto it = colorMap.find(str); it != colorMap.end()) {
		return it->second; // 対応するenum値を返す
	}

	// 見つからなかった場合は空のoptionalを返す
	return std::nullopt;
}

int main() {
	// 擬似的なテキストファイルからの読み込み
	std::string setting1 = "GREEN";
	std::string setting2 = "PURPLE";

	// 設定1の読み込み
	if (auto color_opt = stringToSurfaceType(setting1); color_opt.has_value()) {
		SurfaceType userSurfaceType = color_opt.value();

		// enum値として利用
		if (userSurfaceType == SurfaceType::GREEN) {
			std::cout << "設定1: GREENが正常に読み込まれました。" << std::endl;
		}
	} else {
		std::cout << "設定1: エラー - 不明な色名です。" << std::endl;
	}

	std::cout << "-----------------" << std::endl;

	// 設定2の読み込み（エラーケース）
	if (auto color_opt = stringToSurfaceType(setting2); color_opt.has_value()) {
		// ...
	} else {
		std::cout << "設定2: エラー - 不明な色名 '" << setting2 << "' です。" << std::endl;
	}

	return 0;
}


