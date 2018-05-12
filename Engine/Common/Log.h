#pragma once

#define LOG(item) _CrtDbgReport(_CRT_WARN, NULL, 0, NULL, item);
#define LOGI(item0, item1) _CrtDbgReport(_CRT_WARN, NULL, 0, NULL, item0, item1);

template <typename T>
inline void LOG_SIZE_STRUCT(const char* name, const T& item)
{
	int size = sizeof(item);
	_CrtDbgReport(_CRT_WARN, NULL, 0, NULL, "LOG: class: %s = %d\n", name, size);
}
