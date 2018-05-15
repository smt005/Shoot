#include "Camera.h"
#include "../App/App.h"
#include "../App/File.h"
#include "../Engine/Callback/Callback.h"
#include "../Engine/Physics/PhysicPlane.h"

#include <glm/vec4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

#define FILE_NAME_CAMERA_DATA "Data/Camera.json"

Camera::Camera() :
	_calcFrustum(false)
{
	setDefault();
}

bool Camera::load(const std::string& name)
{
	char *dataString = File::loadText(FILE_NAME_CAMERA_DATA);
	if (!dataString)
		return false;

	json dataAll = json::parse(dataString);

	if (dataAll[name].is_null())
		return false;

	json data = dataAll[name];

	function<void(const string&, vec3&, const vec3&)> readVec = [data](const string& key, vec3& pos, const vec3& defaultPos)
	{
		json posData = data[key];

		if (posData.is_array())
		{
			int index = 0;

			for (auto& item : posData)
			{
				float value = item.is_number_float() ? item.get<float>() : 0.0f;
				pos[index] = value;

				++index;
				if (index > 2)
					break;
			}
		}
	};

	readVec("eye", _eye, vec3(1.0f));
	readVec("target", _target, vec3(0.0f));

	_matView = lookAt(_eye, _target, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();

	return true;
}

void Camera::save(const std::string& name)
{
	char *dataString = File::loadText(FILE_NAME_CAMERA_DATA);
	if (!dataString)
		return;

	json dataAll = json::parse(dataString);

	{
		json posData;

		posData[0] = _eye.x;
		posData[1] = _eye.y;
		posData[2] = _eye.z;

		dataAll[name]["eye"] = posData;
	}

	{
		json posData;

		posData[0] = _target.x;
		posData[1] = _target.y;
		posData[2] = _target.z;

		dataAll[name]["target"] = posData;
	}

	File::saveText(FILE_NAME_CAMERA_DATA, dataAll.dump().c_str());
}

void Camera::setPos(const glm::vec3& pos)
{
	vec3& currentPos = _fromEye ? _eye : _target;
	vec3 offset(currentPos - pos);
	setOffset(-offset);
}

void Camera::setOffset(const glm::vec3& offset)
{
	_target += offset;
	_eye += offset;

	_matView = lookAt(_eye, _target, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::move(const CameraMove direct)
{
	float speed = _fromEye ? -_speed : _speed;
	vec3 offset(0.0f);

	switch (direct)
	{
	case CameraMove::FORVARD:
		offset -= _vec * speed;
		break;

	case CameraMove::BACK:
		offset += _vec * speed;
		break;

	case CameraMove::LEFT:
		offset.x -= (_vec.y * speed);
		offset.y += (_vec.x * speed);
		break;

	case CameraMove::RIGHT:
		offset.x += (_vec.y * speed);
		offset.y -= (_vec.x * speed);
		break;

	case CameraMove::TOP:
		offset.z += abs(speed);
		break;

	case CameraMove::DOWN:
		offset.z -= abs(speed);
		break;

	case CameraMove::BACK_HORIZONT:
		offset.x += (_vec.x * speed);
		offset.y += (_vec.y * speed);
		break;

	case CameraMove::HORIZONT:
		offset.x -= (_vec.x * speed);
		offset.y -= (_vec.y * speed);
		break;
	}

	setOffset(offset);
}

void Camera::move(const vec2& direct)
{
	float speed = _fromEye ? -_speed : _speed;
	vec3 offset(0.0f);

	offset.x += (_vec.y * direct.x * speed);
	offset.y -= (_vec.x * direct.x * speed);

	offset.x -= (_vec.x * direct.y * speed);
	offset.y -= (_vec.y * direct.y * speed);

	setOffset(offset);
}

void Camera::setVector(const vec3& vec)
{
	if (_vec == vec)
		return;

	_vec = vec;
	setVector();
}

void Camera::setVector()
{
	if (_fromEye)
		_target = _eye + _vec * _dist;
	else
		_eye = _target + _vec * (-_dist);

	_matView = lookAt(_eye, _target, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::setDist(const float& dist)
{
	if (_dist == dist)
		return;

	_dist = dist;
	setVector();
}

void Camera::setDefault()
{
	_matProject = mat4x4(1.0f);
	_matView = mat4x4(1.0f);

	_fromEye = true;
	_speed = 0.00001f;
	_speedRotate = 0.01f;

	_matProject = perspective(45.0f, App::aspect(), 0.1f, 1000.0f);
	setLookAt(vec3(25.0f, 25.0f, 25.0f), vec3(0.0f, 0.0f, 0.0f));
}

void Camera::setLookAt(const vec3& eye, const vec3& target)
{
	if (_fromEye)
	{
		_eye = eye;
		_target = target;
	}
	else
	{
		_eye = target;
		_target = eye;
	}

	_vec = target - eye;
	_dist = length(_vec);
	_vec = normalize(_vec);

	_matView = lookAt(eye, target, vec3(0.0f, 0.0f, 1.0f));
	makeMatProjectView();
}

void Camera::setOrtho(const float left, const float right, const float bottom, const float top)
{
	_matProject = ortho(left, right, bottom, top);
	makeMatProjectView();
}

void Camera::setPerspective(const float fov, const float aspect, const float zNear, const float zFar)
{
	_matProject = perspective(fov, aspect, zNear, zFar);
	makeMatProjectView();
}

void Camera::rotate(const vec2& angles)
{
	vec3 vector = _vec;
	double angleY = asinf(vector.z);
	double angleX = acos(vector.y / cos(angleY));

	if (vector.x < 0.0f)
	{
		angleX = glm::pi<float>() + (glm::pi<float>() - angleX);
	}

	angleX = angleX - angles.x * _speedRotate;
	angleY = angleY + angles.y * _speedRotate;

	if (angleY >(glm::pi<float>() / 2.0 - 0.25)) angleY = (glm::pi<float>() / 2.0 - 0.25);
	if (angleY < -(glm::pi<float>() / 2.0 - 0.25)) angleY = -(glm::pi<float>() / 2.0 - 0.25);

	vector.x = sinf(static_cast<float>(angleX)) * cosf(static_cast<float>(angleY));
	vector.y = cosf(static_cast<float>(angleX)) * cosf(static_cast<float>(angleY));
	vector.z = sinf(static_cast<float>(angleY));
	normalize(vector);

	setVector(vector);
}

vec3 Camera::corsorCoord()
{
	glm::vec3 wincoord = glm::vec3(Callback::pos.x - App::pos().x, (App::height() - Callback::pos.y) + App::pos().y, 1.0f);
	glm::vec4 viewport = glm::vec4(0, 0, App::width(), App::height());

	glm::vec3 coord = glm::unProject(wincoord, _matView, _matProject, viewport);

	vec3 vecCursor(_eye.x - coord.x, _eye.y - coord.y, _eye.z - coord.z);
	vecCursor = normalize(vecCursor);

	PhysicPlane plane;
	plane.set(vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));

	vec3 objcoord = plane.crossVector(vecCursor, _eye);

	return objcoord;
};

void Camera::makeFrustum()
{
	float *clip = value_ptr(_matProjectView);

	// Находим A, B, C, D для ПРАВОЙ плоскости
	_frustum[0].x = clip[3] - clip[0];
	_frustum[0].y = clip[7] - clip[4];
	_frustum[0].z = clip[11] - clip[8];
	_frustum[0].w = clip[15] - clip[12];
	normalize(_frustum[0]);

	// Находим A, B, C, D для ЛЕВОЙ плоскости
	_frustum[1].x = clip[3] + clip[0];
	_frustum[1].y = clip[7] + clip[4];
	_frustum[1].z = clip[11] + clip[8];
	_frustum[1].w = clip[15] + clip[12];
	normalize(_frustum[1]);

	// Находим A, B, C, D для НИЖНЕЙ плоскости
	_frustum[2].x = clip[3] + clip[1];
	_frustum[2].y = clip[7] + clip[5];
	_frustum[2].z = clip[11] + clip[9];
	_frustum[2].w = clip[15] + clip[13];
	normalize(_frustum[2]);

	// ВЕРХНЯЯ плоскость
	_frustum[3].x = clip[3] - clip[1];
	_frustum[3].y = clip[7] - clip[5];
	_frustum[3].z = clip[11] - clip[9];
	_frustum[3].w = clip[15] - clip[13];
	normalize(_frustum[3]);

	// ЗАДНЯЯ плоскость
	_frustum[4].x = clip[3] - clip[2];
	_frustum[4].y = clip[7] - clip[6];
	_frustum[4].z = clip[11] - clip[10];
	_frustum[4].w = clip[15] - clip[14];
	normalize(_frustum[4]);

	// ПЕРЕДНЯЯ плоскость
	_frustum[5].x = clip[3] + clip[2];
	_frustum[5].y = clip[7] + clip[6];
	_frustum[5].z = clip[11] + clip[10];
	_frustum[5].w = clip[15] + clip[14];
	normalize(_frustum[5]);
}

//---

vec4 Camera::_frustum[6];
mat4x4 Camera::_matPVM;

Camera Camera::current;

