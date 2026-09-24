
std::string* read_list(std::string path, int &Nelements);
double** read_data(std::string path, std::string &header, int &Nlines, int &Ncolumns);/* 参照渡し */
double** read_fixed_data(std::string path, std::string &header, int &Nlines, int &Ncolumns);/* Nline, Ncolumns がわかっている */

void save_data(std::string path, std::string header, int Nlines, int Ncolumns, double** data);
void save_data(std::string path, std::string header, int Nlines, int Ncolumns, int** data);/* TODO to templete */
void save_data_and_string(std::string path, std::string header, int Nlines, int Ncolumns, double** data, std::string* string);
