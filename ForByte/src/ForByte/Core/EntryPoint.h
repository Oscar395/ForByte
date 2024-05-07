#pragma once
#include "ForByte/Core/Core.h"

#ifdef FB_PLATFORM_WINDOWS

extern ForByte::Application* ForByte::CreateApplication();

int main(int argc, char** argv) 
{
	ForByte::Log::Init();

	FB_PROFILE_BEGIN_SESSION("Startup", "ForByteProfile-Startup.json");
	auto app = ForByte::CreateApplication();
	FB_PROFILE_END_SESSION();

	FB_PROFILE_BEGIN_SESSION("Runtime", "ForByteProfile-Runtime.json");
	app->Run();
	FB_PROFILE_END_SESSION();

	FB_PROFILE_BEGIN_SESSION("Shutdowm", "ForBdyteProfile-Shutdown.json");
	delete app;
	FB_PROFILE_END_SESSION();
}

#endif // FB_PLATFORM_WINDOWS
