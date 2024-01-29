#pragma once

#ifdef FB_PLATFORM_WINDOWS
	#ifdef FB_BUILD_DLL
		#define FORBYTE_API __declspec(dllexport)
	#else
		#define FORBYTE_API __declspec(dllimport)
	#endif // FB_BUILD_DLL
#else
	#error ForByte only supports Windows!
#endif // FB_PLATFORM_WINDOWS

#ifdef FB_ENABLE_ASSERTS
	#define FB_ASSERT(x, ...) {if(!(x)) { FB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debug_break(); } }
	#define FB_CORE_ASSERT(x, ...) {if(!(x)) { FB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debug_break(); } }
#else
	#define FB_ASSERT(x, ...)
	#define FB_CORE_ASSERT(x, ...)
#endif // FB_ENABLE_ASSERTS

#define BIT(x) (1 << x)