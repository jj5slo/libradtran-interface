#ifndef EXECUTE_H_
#define EXECUTE_H_


#include<cstdlib>
#include<string>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>

#include"obsDateTime.h"

const std::string HOME_DIR = std::string(std::getenv("HOME"));

const std::string DIR_INTERFACE = HOME_DIR+"/SANO/research/estimate-profile/libradtran-interface/";
const std::string DIR_UVSPEC { HOME_DIR+"/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/bin/" };


/* execute.cpp */
void execute_uvspec(std::string path_stdin, std::string path_stdout);



/* read.cpp */
const int MYSTIC_RADIANCE_COLUMN = 7;

double read_mystic_rad(int NN);
double read_stdout(std::string path_stdout, int number_of_column);


#endif
