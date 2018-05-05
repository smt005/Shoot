#include "../App/App.h"

int main(int count, char** arg)
{
	char* exeFile = count > 0 ? arg[0] : nullptr;
	return !App::execution(exeFile);
}