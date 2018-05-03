#pragma once

#include "../Common/DataClass.h"
#include "../Common/Json.h"
#include "Shape.h"
#include "Texture.h"

#include <memory>
#include <string>
using namespace std;

using json = nlohmann::json;

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class Model : public DataClass <Model>
{
private:
	ShapePtr _shape;
	TexturePtr _texture;
	float _scale[3];

public:
	virtual ~Model() {};

	Shape& getShape();
	ShapePtr& getShapePtr();
	Mesh& getMesh();
	Texture& texture();
	const unsigned int& textureId();

	bool create(const string &name);

private:
	static json _data;

public:
	static json data(const string &name);
	static void removeData();
};
