#ifndef __CAMERA_H__
#define __CAMERA_H__

#pragma once

#include <string>

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class CameraMove
{
	NONE,
	FORVARD,
	BACK,
	LEFT,
	RIGHT,
	TOP,
	DOWN,
	HORIZONT,
	BACK_HORIZONT
};

class Camera final
{
public:
	Camera();

	bool load(const std::string& name = "default");
	void save(const std::string& name = "default");

	inline const glm::mat4x4& getMatProject()		{ return _matProject; };
	inline const glm::mat4x4& getMatView()			{ return _matView; };
	inline const glm::mat4x4& getMatProjectView()	{ return _matProjectView; };

	inline bool fromEye()		{ return _fromEye; };
	inline bool calcFrustum()	{ return _calcFrustum; };
	inline const glm::vec3& getTarget()	{ return _target; };
	inline const glm::vec3& getEye()	{ return _eye; };
	inline const glm::vec3& getVector()	{ return _vec; };
	inline float getDist()		{ return _dist; };
	inline float speed()		{ return _speed; }
	inline float speedRotate()	{ return _speedRotate; }

	inline const float* matProjectViewFloat()	{ return glm::value_ptr(_matProjectView); }
	inline const float* matP()					{ return glm::value_ptr(_matProject); }
	inline const float* matPV()					{ return glm::value_ptr(_matProjectView); }
	
	inline const float* matPVM(const glm::mat4x4 &matModel)
	{
		_matPVM = _matProjectView * matModel;
		return glm::value_ptr(_matPVM);
	}

	inline void setFromEye(const bool& state)			{ _fromEye = state; };
	inline void setTarget(const glm::vec3& target)		{ _target = target; };
	inline void setEye(const glm::vec3& eye)			{ _eye = eye; };
	inline void setSpeed(const float speed)		{ _speed = speed; };
	inline void setSpeedRotate(const float speedRotate)	{ _speedRotate = speedRotate; };
	inline void setCalcFrustum(const bool& state)		{ _calcFrustum = state; };

	void setVector();
	void setVector(const glm::vec3& vec);
	void setDist(const float& dist);
	
	void setPos(const glm::vec3& pos);
	void setOffset(const glm::vec3& offset);

	void setDefault();
	void setLookAt(const glm::vec3& eye, const glm::vec3& target);
	void setOrtho(const float left, const float right, const float bottom, const float top);
	void setPerspective(const float fov, const float aspect, const float zNear, const float zFar);
	void makeFrustum();

	inline void makeMatProjectView()
	{
		_matProjectView = _matProject * _matView;
		if (_calcFrustum)
			makeFrustum();
	}

	void move(const CameraMove direct);
	void move(const glm::vec2& direct);
	void rotate(const glm::vec2& angles);

	glm::vec3 corsorCoord();

private:
	glm::mat4x4 _matProject;
	glm::mat4x4 _matView;
	glm::mat4x4 _matProjectView;

	glm::vec3 _target;
	glm::vec3 _eye;
	glm::vec3 _vec;
	float _dist;

	bool _calcFrustum;
	bool _fromEye;
	float _speed;
	float _speedRotate;

private:
	static glm::vec4 _frustum[6];
	static glm::mat4x4 _matPVM;

public:
	static Camera current;
};

#endif //	__CAMERA_H__