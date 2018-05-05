#include "File.h"

#include <stdio.h>
#include <Shlobj.h>

string File::_appResourcesDir;

char* File::loadText(const string& fileName)
{
	if (fileName.empty())
		return nullptr;

	string fullPath = _appResourcesDir + fileName;

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

	string fullPath = _appResourcesDir + fileName;

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

	return _appResourcesDir + fileName;
}

void File::setAppResourcesDir(const string& appResourcesDir)
{
	_appResourcesDir = appResourcesDir;
}
