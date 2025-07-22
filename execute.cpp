#include"execute.h"

void execute_uvspec(std::string path_stdin, std::string path_stdout){/* path は full path */
	system(("cd "+DIR_UVSPEC+" && ./uvspec < "+path_stdin+" > "+path_stdout).c_str());
}	
