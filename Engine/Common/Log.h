#pragma once

#define LOG(item) _CrtDbgReport(_CRT_WARN, NULL, 0, NULL, item);
#define LOGI(item0, item1) _CrtDbgReport(_CRT_WARN, NULL, 0, NULL, item0, item1);