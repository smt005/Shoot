#pragma once

#include <string>

using namespace std;

class File
{
public:
	static char* loadText(const string& fileName);
	static void saveText(const string& fileName, const char *data);
	static string getFullPath(const string& fileName);
};

