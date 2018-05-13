#pragma once

#include "Log.h"

#include <string>
#include <map>
#include <memory>

using namespace std;

template <class ObjectT>
#define ObjectPtrT shared_ptr<ObjectT>

class DataClass
{
protected:
	string _name;

public:
	virtual ~DataClass() {};

	virtual bool create(const string &name);
	virtual void setDefault();

	inline void setName(const string& name) { _name = name; };
	inline const string name() { return _name; };

private:
	static map<string, ObjectPtrT>* _map;

public:
	static ObjectPtrT addPtr(const string& name, ObjectT* newItem);
	static ObjectPtrT getByName(const string& name);
	static bool hasByName(const string& name);

	static void erase(const string& name);
	static void clear(bool onlyUnused = true);

	inline static map<string, ObjectPtrT>& getMap()
	{
		if (!_map)
			_map = new map<string, ObjectPtrT>();

		return *_map;
	}
};

template <class ObjectT>
map<string, ObjectPtrT>* DataClass<ObjectT>::_map = nullptr;

template <class ObjectT>
bool DataClass<ObjectT>::create(const string &name)
{
	_name = name;
	return true;
};

template <class ObjectT>
void DataClass<ObjectT>::setDefault()
{
}

template<class ObjectT>
ObjectPtrT DataClass<ObjectT>::addPtr(const string& name, ObjectT* newItem)
{
	if (!newItem)
		return getByName(name);

	ObjectPtrT newItemPtr(newItem);
	pair<string, ObjectPtrT> objectPair(name, newItemPtr);
	getMap().insert(objectPair);

	return newItemPtr;
}

template <class ObjectT>
ObjectPtrT DataClass<ObjectT>::getByName(const string& name)
{
	auto& map = getMap();

	auto it = map.find(name);
	if (it != map.end())
	{
		return it->second;
	}

	ObjectT* newItem = new ObjectT();

	if (!newItem->create(name))
	{
		newItem->setDefault();
	}

	ObjectPtrT newItemPtr(newItem);
	pair<string, ObjectPtrT> objectPair(name, newItemPtr);
	map.insert(objectPair);

	return newItemPtr;
}

template <class ObjectT>
bool DataClass<ObjectT>::hasByName(const string& name)
{
	auto& map = getMap();
	auto it = map.find(name);
	return it != map.end() ? true : false;
}

template <class ObjectT>
void DataClass<ObjectT>::erase(const string& name)
{
	auto& map = getMap();
	auto it = map.find(name);
	if (it != map.end())
		map.erase(it);
}

template <class ObjectT>
void DataClass<ObjectT>::clear(bool onlyUnused)
{
	if (onlyUnused = false)
	{
		getMap().clear();
	}
	else
	{
		auto& map = getMap();
		remove(map.begin(), map.end(), [](ObjectPtrT& itemPtr) { return itemPtr.use_count <= 0 ? true : false; });
	}
}
