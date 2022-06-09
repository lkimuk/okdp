#pragma once
#include <UIlib.h>
#ifdef _DEBUG
#pragma comment(lib, "DuiLib_d.lib")
#else
#pragma comment(lib, "DuiLib.lib")
#endif // _DEBUG

using namespace DuiLib;

#include <memory>
#include <functional>