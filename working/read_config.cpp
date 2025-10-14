/*
 * read_config.cpp
 * 設定をファイルから読み込み
 *
 */

#include"read_config.h"


std::map<std::string, std::string> readConfigFile(const std::string filename){
	std::map<std::string, std::string> configs;
	std::ifstream file(filename);

	if(!file.is_open()){
		std::cerr << "Error: read_config: Cannot open " << filename << std::endl;
		return configs;/* void map */
	}
	
	std::string line;
	while(std::getline(file, line)){	
		// 行頭と行末の空白を削除
		size_t first = line.find_first_not_of(" \t\r\n");
		if (first == std::string::npos) continue; // 空行
		size_t last = line.find_last_not_of(" \t\r\n");
		std::string trimmed_line = line.substr(first, (last - first + 1));
		
		// コメント（#から始まる行）を無視
		if (trimmed_line.empty() || trimmed_line[0] == '#') {
			continue;
		}
		
		// '='の位置を探す
		size_t equal_pos = trimmed_line.find('=');
		if (equal_pos == std::string::npos) {
			continue; // '=' がない行は無視
		}
		
		// キーと値を取得
		std::string key = trimmed_line.substr(0, equal_pos);
		std::string value = trimmed_line.substr(equal_pos + 1);
		
		// キーと値の周りの空白を再度削除
		key.erase(0, key.find_first_not_of(" \t\r\n"));
		key.erase(key.find_last_not_of(" \t\r\n") + 1);
		value.erase(0, value.find_first_not_of(" \t\r\n"));
		value.erase(value.find_last_not_of(" \t\r\n") + 1);
		
		if (!key.empty()) {
			configs[key] = value;
		}
	}
	return configs;
}


std::string getConfig(const std::map<std::string, std::string> configs, 
             const std::string key, /* 必要に応じてすべて参照渡し */
             const std::string defaultValue){/* 値を取り出す */
	auto raw = configs.find(key);
	if( raw != configs.end() ){/* 見つかれば */
		return raw->second;
	}
	else{
		std::cout << "Notice: read_config: No value in configure file. Default value is used. key=" << key << ", value=" << defaultValue << std::endl;
		return defaultValue;
	}
}
int getConfig(const std::map<std::string, std::string> configs, 
             const std::string key, /* 必要に応じてすべて参照渡し */
             const int defaultValue){/* 値を取り出す */
	auto raw = configs.find(key);
	if( raw != configs.end() ){/* 見つかれば */
		std::stringstream ss(raw->second);/* raw の value */
		int value;
		if(ss >> value){
			return value;/* 型変換 */
		}
		else{
			std::cerr << "Error: read_config: getConfig: Invalid value. Default value is used. key=" << key << ", value=" << defaultValue << std::endl;
			return defaultValue;
		}
	}
	else{
		std::cout << "Notice: read_config: No value in configure file. Default value is used. key=" << key << ", value=" << defaultValue << std::endl;
		return defaultValue;
	}
}
double getConfig(const std::map<std::string, std::string> configs, 
             const std::string key, /* 必要に応じてすべて参照渡し */
             const double defaultValue){/* 値を取り出す */
	auto raw = configs.find(key);
	if( raw != configs.end() ){/* 見つかれば */
		std::stringstream ss(raw->second);/* raw の value */
		double value;
		if(ss >> value){
			return value;/* 型変換 */
		}
		else{
			std::cerr << "Error: read_config: getConfig: Invalid value. Default value is used. key=" << key << ", value=" << defaultValue << std::endl;
			return defaultValue;
		}
	}
	else{
		std::cout << "Notice: read_config: No value in configure file. Default value is used. key=" << key << ", value=" << defaultValue << std::endl;
		return defaultValue;
	}
}
