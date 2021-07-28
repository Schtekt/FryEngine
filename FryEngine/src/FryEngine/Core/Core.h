#pragma once

#ifdef FRY_BUILD_DLL
#define FRY_EXPORT __declspec(dllexport)
#else
#define FRY_EXPORT __declspec(dllimport)
#endif