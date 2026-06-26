#ifndef _FILEMATCH_H_
#define _FILEMATCH_H_



void deleteMatchingFiles(const std::string& dirPath, const std::regex& TARGET_PATTERN);
void listMatchingFiles(const std::string& dirPath, const std::regex& TARGET_PATTERN, std::string*& list, int& Nfiles);



#endif
