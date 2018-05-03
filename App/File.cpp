#include "File.h"

string appResourcesDir("X:/Projects/Shoot/Debug/Resources/");

char* File::loadText(const string& fileName)
{
	if (fileName.empty())
		return nullptr;

	string fullPath = appResourcesDir + fileName;

	FILE* file;
	char* buf = nullptr;

	if (fopen_s(&file, fullPath.c_str(), "r") == 0)
	{
		fseek(file, 0, SEEK_END);
		int lenght = ftell(file);
		rewind(file);

		if (lenght == 0)
		{
			fclose(file);
			return nullptr;
		}

		buf = (char *)malloc(sizeof(char)* (lenght + 1));
		lenght = fread(buf, sizeof(char), lenght, file);
		buf[lenght] = '\0';

		fclose(file);
	}

	return buf;
}


void File::saveText(const string& fileName, const char *data)
{
	if (fileName.empty())
		return;

	string fullPath = appResourcesDir + fileName;

	FILE* file;

	if (fopen_s(&file, fullPath.c_str(), "w") !=  0)
		return;

	fputs(data, file);
	fclose(file);
}

string File::getFullPath(const string& fileName)
{
	if (fileName.empty())
		return string();

	return appResourcesDir + fileName;
}