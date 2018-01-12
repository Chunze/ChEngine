#include "FileManager.h"


FileManager::FileManager()
{

}

std::string FileManager::ReadTextFileAsString(const char* FileName)
{
	std::ifstream ShaderFile(FileName);

	if (!ShaderFile.good())
	{
		std::cout << "ERROR::SHADER::LOADING::Fail to load shader from file" << FileName << std::endl;
		return "";
	}

	return std::string(
		std::istreambuf_iterator<char>(ShaderFile),
		std::istreambuf_iterator<char>()
	);
}

