#include "save.h"

int save_params(std::string dir, std::string secid, std::string path_stdin, std::string suffix){
	std::string dest = dir + "/" + secid + suffix;
	try {
		std::filesystem::copy(path_stdin, dest, std::filesystem::copy_options::overwrite_existing);
		std::cout << "File copied successfully." << std::endl;
	} catch (const std::filesystem::filesystem_error& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}
	return 0;
}

