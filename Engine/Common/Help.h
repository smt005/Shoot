#pragma once

#include "../Common/IncludesMatem.h"
#include <vector>

namespace help
{
using namespace glm;
using namespace std;

bool intersection(vec3 start1, vec3 end1, vec3 start2, vec3 end2, vec3 *out_intersection);
float random_f(const float& min  = 0.0f, const float& max = 1.0f, const int& accuracy = 1000);
int random_i(const int& min = 0, const int& max = 1);

template <class ObjectT>
ObjectT& add(vector<ObjectT*>& vector)
{
	ObjectT* newItem = new ObjectT();
	vector.push_back(newItem);
	return *newItem;
}

template <class ObjectT>
void erase(vector<ObjectT*>& vector, const ObjectT item)
{
	auto it = find(vector.begin(), vector.end(), item);

	if (it != vector.end())
	{
		vector.erase(it);
	}
}

template <class ObjectT>
void erase(vector<ObjectT*>& vector, const string& name)
{
	auto it = find_if(vector.begin(), vector.end(), [name](auto item)
	{
		if (!item)
			return false;

		if (item->name() == name)
			return true;
	});

	if (it != vector.end())
	{
		delete it->get();
		vector.erase(it);
	}
}

template <class ObjectT>
void clear(vector<ObjectT*>& vector)
{
	for (auto object : vector)
	{
		delete object;
	}

	vector.clear();
}

template <class ObjectT>
ObjectT& find(vector<ObjectT*>& vector, const string& name)
{
	auto it = find_if(vector.begin(), vector.end(), [name](auto item)
	{
		if (!item)
			return false;

		if (item->name() == name)
			return true;

		return false;
	});

	if (it != vector.end())
	{
		return *(*it);
	}

	return ObjectT::defaultItem();
}

}

/*
//_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "KOP LOG: %f\n", 0.0f);
*/