#pragma once

#include <memory>

#ifdef FB_PLATFORM_WINDOWS
#if FB_DYNAMIC_LINK
	#ifdef FB_BUILD_DLL
		#define FORBYTE_API __declspec(dllexport)
	#else
		#define FORBYTE_API __declspec(dllimport)
	#endif // FB_BUILD_DLL
#else
	#define FORBYTE_API
#endif
#else
	#error ForByte only supports Windows!
#endif // FB_PLATFORM_WINDOWS

#ifdef FB_DEBUG
	#define FB_ENABLE_ASSERTS
#endif

#ifdef FB_ENABLE_ASSERTS
	#define FB_ASSERT(x, ...) {if(!(x)) { FB_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define FB_CORE_ASSERT(x, ...) {if(!(x)) { FB_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define FB_ASSERT(x, ...)
	#define FB_CORE_ASSERT(x, ...)
#endif // FB_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define FB_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace ForByte {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}