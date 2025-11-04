#include"execute.h"

void execute_uvspec(const std::string DIR_UVSPEC, const std::string path_stdin, const std::string path_stdout, const int FLAG_UNDISPLAY_LOG, const std::string DIR_LOG){/* path は full path */
	if(FLAG_UNDISPLAY_LOG){
		system(("cd "+DIR_UVSPEC+" && ./uvspec < "+path_stdin+" > "+path_stdout+" 2>> "+DIR_LOG+"libRadtran.log").c_str());
	}else{
		system(("cd "+DIR_UVSPEC+" && ./uvspec < "+path_stdin+" > "+path_stdout).c_str());
	}
}	
