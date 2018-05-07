#pragma once

#include "../Common/DataClass.h"

#include <string>
#include <memory>
using namespace std;

#define SHAPE_VERTEX_POS_SIZE	3
#define SHAPE_VERTEX_TEX_SIZE   2

// TODO:
class PhysicObject;

class btCollisionShape;
class btBvhTriangleMeshShape;
class Shape;
typedef std::shared_ptr<Shape> ShapePtr;

struct MeshTemporary;

class Mesh
{
public:
	unsigned short int _countVertex = 0;
	float* _aVertex = nullptr;
	float* _aNormal = nullptr;
	float* _aTexCoord = nullptr;

	unsigned short int _countIndex = 0;
	unsigned short* _aIndex = nullptr;

	bool _hasVBO = false;
	unsigned int _buffer[4];

public:
	virtual ~Mesh();
	void initVBO();
};

class MeshPhysic
{
	// TODO:
	friend PhysicObject;

public:
	unsigned short int _count = 0;
	Mesh* _meshes = nullptr;

	btCollisionShape* _collisionShape = nullptr;
	btBvhTriangleMeshShape* _triangleMeshShape = nullptr;

public:
	virtual ~MeshPhysic();
};

class Shape: public Mesh, public DataClass<Shape>
{
private:
	MeshPhysic* _physic = nullptr;

public:
	virtual ~Shape();
    
	bool create(const string &name) override;
	bool load(const string& name);

	MeshPhysic* getPhysic() { return _physic; };
	void setScale(float* scale);
};
