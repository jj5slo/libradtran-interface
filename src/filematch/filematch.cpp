#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>

#include "readwrite.h"

void deleteMatchingFiles(const std::string& dirPath, const std::regex& TARGET_PATTERN){
	if(!std::filesystem::exists(dirPath)){ return; }
	for(const auto& entry : std::filesystem::directory_iterator(dirPath)){
		if(entry.is_regular_file()){
			std::string filename = entry.path().filename().string();
			if(std::regex_match(filename, TARGET_PATTERN)){
				std::cout << "[deleted] " << entry.path().filename() << std::endl;
				std::filesystem::remove(entry.path());
			}
		}
	}
}

void listMatchingFiles(const std::string& dirPath, const std::regex& TARGET_PATTERN, std::string*& list, int& Nfiles){
	if(!std::filesystem::exists(dirPath)){ return; }
	Nfiles = 0;
	for(const auto& entry : std::filesystem::directory_iterator(dirPath)){
		if(entry.is_regular_file()){
			std::string filename = entry.path().filename().string();
			if(std::regex_match(filename, TARGET_PATTERN)){
				Nfiles = Nfiles + 1;
			}
		}
	}
//	if(list != nullptr){ delete[] list; }
	list = new std::string[Nfiles];
	int i=0;
	for(const auto& entry : std::filesystem::directory_iterator(dirPath)){
		if(entry.is_regular_file()){
			std::string filename = entry.path().filename().string();
			if(std::regex_match(filename, TARGET_PATTERN)){
				list[i] = filename;
				i = i + 1;
			}
		}
	}
}
