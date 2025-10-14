#ifndef __READ_CONFIG_H__
#define __READ_CONFIG_H__

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<algorithm>

std::map<std::string, std::string> readConfigFile(const std::string filename);/* 構成ファイル読込 */


std::string getConfig(const std::map<std::string, std::string> configs, 
             const std::string key, /* 必要に応じてすべて参照渡し */
             const std::string defaultValue);

int getConfig(const std::map<std::string, std::string> configs, 
             const std::string key, /* 必要に応じてすべて参照渡し */
             const int defaultValue);

double getConfig(const std::map<std::string, std::string> configs, 
             const std::string key, /* 必要に応じてすべて参照渡し */
             const double defaultValue);


#endif
