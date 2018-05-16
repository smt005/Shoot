#ifndef __JSONHELP_H__
#define __JSONHELP_H__

#pragma once;

#include "../App/File.h"

#include "Json.h"
using json = nlohmann::json;

class JsonHelp
{
public:
	static bool load(json& jsonData, const std::string& fileName)
	{
		char* charData = File::loadText(fileName);
		if (!charData)
			return false;

		jsonData = json::parse(charData);
		delete[] charData;

		return true;
	}

	inline static bool has(json& jsonData, const string& key)
	{
		return jsonData[key].is_null() ? true : false;
	}
	
	inline static std::string getString(json& jsonData, const std::string& key, const std::string& defaultText = std::string())
	{
		return jsonData[key].is_string() ? jsonData[key].get<std::string>() : defaultText;
	}

	inline static int getInt(json& jsonData, const std::string& key, const int defaultValue = 0)
	{
		return jsonData[key].is_number_integer() ? jsonData[key].get<int>() : defaultValue;
	}

	inline static unsigned int getUnsignedInt(json& jsonData, const std::string& key, const unsigned int defaultValue = 0)
	{
		return jsonData[key].is_number_unsigned() ? jsonData[key].get<unsigned int>() : defaultValue;
	}

	inline static float getFloat(json& jsonData, const std::string& key, const float defaultValue = 0.0f)

	{
		return jsonData[key].is_number_float() ? jsonData[key].get<float>() : defaultValue;
	}

	inline static bool getBool(json& jsonData, const std::string& key, const bool defaultValue = false)
	{
		return jsonData[key].is_boolean() ? jsonData[key].get<bool>() : defaultValue;
	}

	inline static json getArray(json& jsonData, const std::string& key, const json& defaultValue = json::basic_json())
	{
		if (jsonData[key].is_array())
			return jsonData[key];
		else
			return defaultValue;
	}

	/*inline static json getDict(json& jsonData, const std::string& key, const json& defaultValue = json::basic_json())
	{
		if (jsonData[key].is_structured()) // ???
			return jsonData[key];
		else
			return defaultValue;
	}*/
};

#endif // __JSONHELP_H__
