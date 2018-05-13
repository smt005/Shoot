#ifndef __GLIDER_TEMPLATE_H__
#define __GLIDER_TEMPLATE_H__

#include "../Common/DataClass.h"
#include "../Common/Json.h"

using json = nlohmann::json;

#include <memory>
using namespace std;

struct GliderTemplate;
typedef shared_ptr<GliderTemplate> GliderTemplatePtr;

class GliderTemplate : public DataClass<GliderTemplate>
{
public:
	bool create(const string &name);

public:
	static json data(const string &name);
	static void removeData();

private:
public:
	float maxHeight = 4.0f;
	float minHeight = 3.0f;
	float speedHeight = 0.01f;
	float speed = 0.05f;
	float speedRotate = 0.1f;

	float maxSpeed = 0.1f;

private:
	static json _data;
};

#endif	// __GLIDER_TEMPLATE_H__