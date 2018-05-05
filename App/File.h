#pragma once

#include <string>

using namespace std;

class File final
{
public:
	static char* loadText(const string& fileName);
	static void saveText(const string& fileName, const char *data);
	static string getFullPath(const string& fileName);
	static void setAppResourcesDir(const string& resourcesDir);

private:
	static string _appResourcesDir;
};

