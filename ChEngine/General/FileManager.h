#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <fstream>
#include <string.h>
#include <iostream>

class FileManager
{
public:

	FileManager();

	static std::string ReadTextFileAsString(const char* FileName);
};


#endif
