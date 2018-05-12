#include "Model.h"
#include "../../App/File.h"
#include "../Common/Log.h"

#define FILE_NAME_MODEL_DATA	"Models/Models.json"
#define FILE_NAME_SHAPE_FILE	"Models/Box1.obj"
#define FILE_NAME_TEXTURE_FILE	"Textures/BlackWhite.png"

Shape& Model::getShape()
{
	if (_shape) return *_shape;
	_shape = Shape::getByName(_name);

	return *_shape;
}

ShapePtr& Model::getShapePtr()
{
	if (!_shape)
	_shape = Shape::getByName(_name);
	return _shape;
}

Mesh& Model::getMesh()
{
	if (_shape) return *_shape;
	_shape = Shape::getByName(_name);
	return *_shape;
}

Texture& Model::texture()
{
	if (_texture) return *_texture;
	_texture = Texture::getByName(_name);
	return *_texture;
}

const unsigned int& Model::textureId()
{
	if (_texture) return _texture->id();
	_texture = Texture::getByName(_name);
	return _texture->id();
}

bool Model::create(const string &newName)
{
	setName(newName);

	json dataModel = data(newName);
	if (dataModel.empty())
		return false;

	const string &nameShape = dataModel["shape"].is_string() ? dataModel["shape"] : FILE_NAME_SHAPE_FILE;
	const string &nameTexture = dataModel["texture"].is_string() ? dataModel["texture"] : FILE_NAME_TEXTURE_FILE;

	bool hasScalling = false;
	string suffixScale;

	if (dataModel["scale"].is_number_float())
	{
		float value = dataModel["scale"].get<float>();
		_scale[0] = value;
		_scale[1] = value;
		_scale[2] = value;

		suffixScale = std::to_string(value);

		hasScalling = true;
	}
	else if (dataModel["scale"].is_array())
	{
		int index = 0;
		for (float value : dataModel["scale"])
		{
			_scale[index] = value;

			if (index >= 2) break;
			++index;

			string scaleString = std::to_string(value);
			if (suffixScale.empty())
			{
				suffixScale += scaleString;
			}
			else
			{
				suffixScale = suffixScale + "," + scaleString;
			}
		}

		hasScalling = true;
	}

	if (!hasScalling)
	{
		_shape = Shape::getByName(nameShape);
	}
	else
	{
		string nameWithSuffixScale = nameShape + "_[" + suffixScale + "]";

		if (Shape::hasByName(nameWithSuffixScale))
		{
			_shape = Shape::getByName(nameWithSuffixScale);
		}
		else
		{
			Shape* newShape = nullptr;

			if (Shape::hasByName(nameShape))
			{
				_shape = Shape::getByName(nameShape);
				newShape = new Shape(*_shape);
			}
			else
			{
				newShape = new Shape();
				newShape->load(nameShape);
			}

			newShape->setScale(_scale);
			newShape->setName(nameWithSuffixScale);

			_shape = Shape::addPtr(nameWithSuffixScale, newShape);
		}
	}
	
	_texture = Texture::getByName(nameTexture);

	return true;
}

// STATIC

json Model::_data;

json Model::data(const string &name)
{
	if (_data.empty())
	{
		char *dataString = File::loadText(FILE_NAME_MODEL_DATA);
		if (!dataString) return json::basic_json();

		_data = json::parse(dataString);
	}

	return _data[name].is_structured() ? _data[name] : json::basic_json();
}

void Model::removeData()
{
	_data.clear();
}
