#pragma once
#ifdef _SWA_ENGINE_EXPORTS
#define SWA_ENGINE_API __declspec(dllexport)
#else
#define SWA_ENGINE_API __declspec(dllimport)
#endif