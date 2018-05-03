#pragma once

#include "../Common/DataClass.h"

#include <memory>
#include <string>
using namespace std;

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Texture: public DataClass <Texture>
{
private:
	string _fileName;
	unsigned int _id = 0;

public:
	Texture();
	Texture(const string &name, bool needLoad = true);
	virtual ~Texture();

	const unsigned int& id();
	unsigned int load();
	unsigned int load(const string &newName);
};
