#include"read_config.h"

int main(void){
	std::map<std::string, std::string> configs = readConfigFile("./config.conf");
	int a = getConfig(configs, "a", 15);
	double b = getConfig(configs, "b", 12.10);
	std::string c = getConfig(configs, "c", std::string("this is default"));
	std::cout << a << " " << b << " " << c << std::endl;
	return 0;
}


