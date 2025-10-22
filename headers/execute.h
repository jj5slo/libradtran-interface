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



/* execute.cpp */
void execute_uvspec(const std::string DIR_UVSPEC, std::string path_stdin, std::string path_stdout, int FLAG);



/* read.cpp */
const int MYSTIC_RADIANCE_COLUMN = 7;/* radiance は7列目に出力される */

double read_mystic_rad(const std::string DIR_UVSPEC, int NN);
double read_stdout(std::string path_stdout, int number_of_column);


#endif
