
#include "Shape.h"
#include "GL/glew.h"
#include "../../App/File.h"
#include "../Common/Log.h"

//	Mesh

Mesh::~Mesh()
{
	_countVertex = 0;
	
	if (_aVertex)
	{
		delete[] _aVertex;
		_aVertex = nullptr;
	}

	if (_aNormal)
	{
		delete[] _aNormal;
		_aNormal = nullptr;
	}

	if (_aTexCoord)
	{
		delete[] _aTexCoord;
		_aTexCoord = nullptr;
	}

	_countIndex = 0;

	if (_aIndex)
	{
		delete[] _aIndex;
		_aIndex = nullptr;
	}
    
    if (_buffer[0])
    {
        glDeleteBuffers(1, &_buffer[0]);
    }
    
    if (_buffer[1])
    {
        glDeleteBuffers(1, &_buffer[1]);
    }
    
    if (_buffer[2])
    {
        glDeleteBuffers(1, &_buffer[2]);
    }
    
    if (_buffer[3])
    {
        glDeleteBuffers(1, &_buffer[3]);
    }
}

void Mesh::initVBO()
{
	glDeleteBuffers(4, _buffer);

	glGenBuffers(4, _buffer);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aVertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 2 * sizeof(GLfloat), _aTexCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aNormal, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countIndex * sizeof(GLushort), _aIndex, GL_STATIC_DRAW);

	_hasVBO = true;
}

//	MeshPhysic

MeshPhysic::~MeshPhysic()
{
	_count = 0;
	if (_meshes)
	{
		delete[] _meshes;
		_meshes = nullptr;
	}
}

//	Shape

struct BlockTemporary
{
	char* _name = nullptr;
	const int _maxCount = 10240;
	int _countLine = 0;
	char** _charLine = nullptr;

	int _countNumber = 9;

	int _count = 0;
	float* _floatArray = nullptr;
	unsigned short int* _intArray = nullptr;

	BlockTemporary()
	{
		_charLine = new char*[_maxCount];
	}

	BlockTemporary(int countNumber)
	{
		_countNumber = countNumber;
		_charLine = new char*[_maxCount];
	}

	~BlockTemporary()
	{
        delete[] _floatArray;
        delete[] _intArray;
		delete[] _charLine;
	}

	void parse()
	{
		_count = _countLine * _countNumber;

		if (_countNumber == 2 || _countNumber == 3)
		{
			_floatArray = new float[_count];

			for (int i = 0; i < _countLine; ++i)
			{
				int index = i * _countNumber;

				if (_countNumber == 2)
				{
					sscanf_s(_charLine[i], "%f %f", &_floatArray[index], &_floatArray[index + 1]);
				}
				else if (_countNumber == 3)
				{
					sscanf_s(_charLine[i], "%f %f %f", &_floatArray[index], &_floatArray[index + 1], &_floatArray[index + 2]);
				}
			}
		}
		else if (_countNumber == 9)
		{
			_intArray = new unsigned short int[_count];

			for (int i = 0; i < _countLine; ++i)
			{
				int intTemp[9];

				sscanf_s(_charLine[i], "%d/%d/%d %d/%d/%d %d/%d/%d",
					&intTemp[0],
					&intTemp[1],
					&intTemp[2],
					&intTemp[3],
					&intTemp[4],
					&intTemp[5],
					&intTemp[6],
					&intTemp[7],
					&intTemp[8]);

				int index = i * _countNumber;

				for (int i2 = 0; i2 < 9; ++i2)
				{
					_intArray[index + i2] = static_cast<unsigned short int>(intTemp[i2] - 1);
				}
			}
		}
	}

	static int foundIndex(float *vertex, float *normal, float *texCoord,
		float *aVertex, float *aNormal, float *aTexCoord,
		int count)
	{
		for (int index = 0; index < count; ++index)
		{
			if ((vertex[0] == aVertex[index * 3] && vertex[1] == aVertex[(index * 3) + 1] && vertex[2] == aVertex[(index * 3) + 2]) &&
				(normal[0] == aNormal[index * 3] && normal[1] == aNormal[(index * 3) + 1] && normal[2] == aNormal[(index * 3) + 2]) &&
				(texCoord[0] == aTexCoord[index * 2] && texCoord[1] == aTexCoord[(index * 2) + 1]))
			{
				return index;
			}
		}

		return -1;
	};

	static void getMesh(Mesh& mesh, int countIndexTemporary, BlockTemporary* indexTemporarys,
		BlockTemporary& vertexTemporary, BlockTemporary& normalTemporary, BlockTemporary& textureTemporary)
	{
		int countIndexT = 0;
		
		for (int iT = 0; iT < countIndexTemporary; ++iT)
		{
			countIndexT += indexTemporarys[iT]._count;
		}

		// TODO
		unsigned short* aIndexNew = new unsigned short[countIndexT * 3];
		float* aVertexNew = new float[countIndexT * 3];
		float* aNormalNew = new float[countIndexT * 3];
		float* aTextureNew = new float[countIndexT * 2];
		
		int indexVertexNew = 0;
		int iIndexNew = 0;

		float* aVertexT = vertexTemporary._floatArray;
		float* aTextureT = textureTemporary._floatArray;
		float* aNormalT = normalTemporary._floatArray;

		for (int iT = 0; iT < countIndexTemporary; ++iT)
		{
			BlockTemporary& indexTemporary = indexTemporarys[iT];
			unsigned short* aIndexT = indexTemporary._intArray;
			
			for (int i = 0; i < indexTemporary._count; i+=3)
			{
				unsigned short indexV = aIndexT[i];
				float* vertex = &aVertexT[indexV * 3];

				unsigned short indexT = aIndexT[i + 1];
				float* texCoord = &aTextureT[indexT * 2];

				unsigned short indexN = aIndexT[i + 2];
				float* normal = &aNormalT[indexN * 3];

				int index = foundIndex(vertex, normal, texCoord, aVertexNew, aNormalNew, aTextureNew, indexVertexNew);
				if (index == -1)
				{
					index = indexVertexNew;

					aVertexNew[index * 3] = vertex[0];
					aVertexNew[index * 3 + 1] = vertex[1];
					aVertexNew[index * 3 + 2] = vertex[2];

					aNormalNew[index * 3] = normal[0];
					aNormalNew[index * 3 + 1] = normal[1];
					aNormalNew[index * 3 + 2] = normal[2];

					aTextureNew[index * 2] = texCoord[0];
					aTextureNew[index * 2 + 1] = texCoord[1];

					++indexVertexNew;
				}

				aIndexNew[iIndexNew] = index;
				++iIndexNew;
			}
		}
        
		mesh._countIndex = iIndexNew;
		mesh._aIndex = aIndexNew;

		mesh._countVertex = indexVertexNew;
		mesh._aVertex = new float[indexVertexNew * 3];
		mesh._aNormal = new float[indexVertexNew * 3];
		mesh._aTexCoord = new float[indexVertexNew * 2];

		for (int i = 0; i < indexVertexNew; ++i)
		{
			int indexV = i * 3;

			mesh._aVertex[indexV] = aVertexNew[indexV];
			mesh._aVertex[indexV + 1] = aVertexNew[indexV + 1];
			mesh._aVertex[indexV + 2] = aVertexNew[indexV + 2];

			mesh._aNormal[indexV] = aNormalNew[indexV];
			mesh._aNormal[indexV + 1] = aNormalNew[indexV + 1];
			mesh._aNormal[indexV + 2] = aNormalNew[indexV + 2];

			int indexT = i * 2;

			mesh._aTexCoord[indexT] = aTextureNew[indexT];
			mesh._aTexCoord[indexT + 1] = aTextureNew[indexT + 1];
		}

		delete[] aVertexNew;
		delete[] aNormalNew;
		delete[] aTextureNew;
	}
};

Shape::Shape(const Shape& shape)
{
	copy(shape);
}

Shape::~Shape()
{
	if (_physic)
	{
		delete _physic;
		_physic = nullptr;
	}
}

Shape& Shape::operator=(const Shape& shape)
{
	copy(shape);
	return *this;
}

void Shape::copy(const Shape& shape)
{
	// Mesh
	int size(0);
	size = sizeof(shape._aVertex);

	if (shape._countVertex == 0 || shape._countIndex == 0)
		return;

	_countVertex = shape._countVertex;

	int sizeFloat = sizeof(float);

	size = _countVertex * sizeFloat * 3;
	_aVertex = new float[size];
	memcpy(_aVertex, shape._aVertex, size);

	_aNormal = new float[size];
	memcpy(_aNormal, shape._aNormal, size);

	size = _countVertex * sizeFloat * 2;
	_aTexCoord = new float[size];
	memcpy(_aTexCoord, shape._aTexCoord, size);

	_countIndex = shape._countIndex;

	size = sizeof(shape._aIndex);

	size = _countIndex * sizeof(unsigned short) * 3;

	_aIndex = new unsigned short[size];
	memcpy(_aIndex, shape._aIndex, size);
}

bool Shape::create(const string &name)
{
	_name = name;
	return load(_name);
}

bool Shape::load(const string& name)
{
	char* data = File::loadText(name);

	if (!data)
		return false;

	int len = strlen(data);
	int iChar = 0;

	BlockTemporary vertexTemporary(3);
	BlockTemporary normalTemporary(3);
	BlockTemporary textureTemporary(2);

	const int maxCountIndexTemporary = 10;
	BlockTemporary* currentIndexTemporary = nullptr;

	int countIndexTemporary = 0;
	BlockTemporary indexTemporary[maxCountIndexTemporary];

	int countIndexPhysicTemporary = 0;
	BlockTemporary indexPhysicTemporary[maxCountIndexTemporary];
	
	// -
	while (iChar < len)
	{
		if (data[iChar] == 'v' && data[iChar + 1] == ' ' && data[iChar + 2] == ' ')
		{
			vertexTemporary._charLine[vertexTemporary._countLine] = &data[iChar + 3];
			++vertexTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'v' && data[iChar + 1] == 'n' && data[iChar + 2] == ' ')
		{
			normalTemporary._charLine[normalTemporary._countLine] = &data[iChar + 3];
			++normalTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'v' && data[iChar + 1] == 't' && data[iChar + 2] == ' ')
		{
			textureTemporary._charLine[textureTemporary._countLine] = &data[iChar + 3];
			++textureTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'f' && data[iChar + 1] == ' ')
		{
			if (!currentIndexTemporary)
			{
				char* name = nullptr;
				int iCharTemp = iChar - 1;
				data[iCharTemp] = '\0';

				while (iCharTemp >= 0 && data[iCharTemp] != ' ')
				{
					--iCharTemp;
				}

				name = &data[iCharTemp + 1];

				if (name[0] == '_')
				{
					currentIndexTemporary = &indexPhysicTemporary[countIndexPhysicTemporary];
					++countIndexPhysicTemporary;

					if (countIndexPhysicTemporary >= maxCountIndexTemporary)
					{
						break;
					}
				}
				else
				{
					currentIndexTemporary = &indexTemporary[countIndexTemporary];
					++countIndexTemporary;

					if (countIndexPhysicTemporary >= maxCountIndexTemporary)
					{
						break;
					}
				}
				
				currentIndexTemporary->_name = name;
			}

			currentIndexTemporary->_charLine[currentIndexTemporary->_countLine] = &data[iChar + 2];
			++currentIndexTemporary->_countLine;
			iChar += 2;
		}

		if (currentIndexTemporary && (data[iChar] == '#' && data[iChar + 1] == ' '))
		{
			currentIndexTemporary = nullptr;
			iChar += 2;
		}

		if (data[iChar] == '\n')
		{
			data[iChar] = '\0';
		}

		++iChar;
	}
    
	vertexTemporary.parse();
	normalTemporary.parse();
	textureTemporary.parse();

	for (int i = 0; i < countIndexTemporary; ++i)
	{
		indexTemporary[i].parse();
	}

	for (int i = 0; i < countIndexPhysicTemporary; ++i)
	{
		indexPhysicTemporary[i].parse();
	}

	BlockTemporary::getMesh(*this, countIndexTemporary, indexTemporary, vertexTemporary, normalTemporary, textureTemporary);

	if (countIndexPhysicTemporary > 0)
	{
		_physic = new MeshPhysic();

		_physic->_count = countIndexPhysicTemporary;
		_physic->_meshes = new Mesh[countIndexPhysicTemporary];

		for (int i = 0; i < countIndexPhysicTemporary; ++i)
		{
			BlockTemporary::getMesh(_physic->_meshes[i], 1, &indexPhysicTemporary[i], vertexTemporary, normalTemporary, textureTemporary);
		}
	}

	_name = name;
	delete [] data;
	return true;
}

void Shape::setScale(float* scale)
{
	for (int i = 0; i < _countVertex * 3; i+=3)
	{
		_aVertex[i    ] = _aVertex[i    ] * scale[0];
		_aVertex[i + 1] = _aVertex[i + 1] * scale[1];
		_aVertex[i + 2] = _aVertex[i + 2] * scale[2];
	}

	if (_physic)
	{
		for (int i = 0; i < _physic->_count; ++i)
		{
			Mesh& mesh = _physic->_meshes[i];

			for (int i = 0; i < mesh._countVertex * 3; i += 3)
			{
				mesh._aVertex[i    ] = mesh._aVertex[i    ] * scale[0];
				mesh._aVertex[i + 1] = mesh._aVertex[i + 1] * scale[1];
				mesh._aVertex[i + 2] = mesh._aVertex[i + 2] * scale[2];
			}
		}
	}
}
