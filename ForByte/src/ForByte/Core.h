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
