#include <memory_allocate.h> /* AndoLab */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#ifndef __READWRITE_H__
#define __READWRITE_H__

namespace readwrite{
	std::string* read_list(std::string path, int &Nelements);
	double** read_data(std::string path, std::string &header, int &Nlines, int &Ncolumns);/* 参照渡し */
	double** read_fixed_data(std::string path, std::string &header, int Nlines, int Ncolumns);/* Nline, Ncolumns がわかっている */
	double** read_fixed_data_with_skip(std::string path, std::string &header, int Nlines, int Ncolumns, int skipNlines, int skipNcolumns);
	
	void save_data(std::string path, std::string header, int Nlines, int Ncolumns, double** data);
	void save_data(std::string path, std::string header, int Nlines, int Ncolumns, int** data);/* TODO to templete */
	void save_data_and_string(std::string path, std::string header, int Nlines, int Ncolumns, double** data, std::string* string);
	void save_string_and_data(std::string path, std::string header, int Nlines, int Ncolumns, std::string* string, double** data);
}

#endif
