#pragma once

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
	DataClass() {};
	virtual ~DataClass() {};

	virtual bool create(const string &name);
	virtual void setDefault();

	void setName(const string& name) { _name = name; };
	const string name() { return _name; };

private:
	static map<string, ObjectPtrT> _map;
	static ObjectT _default;

public:
	static ObjectPtrT addPtr(const string& name, ObjectT* newItem);
	static ObjectPtrT getByName(const string& name);
	static bool hasByName(const string& name);

	static void erase(const string& name);
	static void clear(bool onlyUnused = true);
};

template <class ObjectT>
map<string, ObjectPtrT> DataClass<ObjectT>::_map;

template <class ObjectT>
ObjectT DataClass<ObjectT>::_default;

template <class ObjectT>
bool DataClass<ObjectT>::create(const string &name)
{
	_name = name;
	return true;
};

template <class ObjectT>
void DataClass<ObjectT>::setDefault()
{
	setName("DEFAULT_DATA_CLASS");
}

template<class ObjectT>
ObjectPtrT DataClass<ObjectT>::addPtr(const string& name, ObjectT* newItem)
{
	if (!newItem)
		return getByName(name);

	ObjectPtrT newItemPtr(newItem);
	pair<string, ObjectPtrT> objectPair(name, newItemPtr);
	_map.insert(objectPair);

	return newItemPtr;
}

template <class ObjectT>
ObjectPtrT DataClass<ObjectT>::getByName(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end())
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
	_map.insert(objectPair);

	return newItemPtr;
}

template <class ObjectT>
bool DataClass<ObjectT>::hasByName(const string& name)
{
	auto it = _map.find(name);
	return it != _map.end() ? true : false;
}

template <class ObjectT>
void DataClass<ObjectT>::erase(const string& name)
{
	auto it = _map.find(name);

	if (it != _map.end())
	{
		_map.erase(it);
	}
}

template <class ObjectT>
void DataClass<ObjectT>::clear(bool onlyUnused)
{
	if (onlyUnused = false)
	{
		_map.clear();
	}
	else
	{
		remove(_map.begin(), _map.end(), [](ObjectPtrT& itemPtr) { return itemPtr.use_count <= 0 ? true : false; });
	}
}
