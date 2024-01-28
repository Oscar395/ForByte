#pragma once

#ifdef FB_PLATFORM_WINDOWS

extern ForByte::Application* ForByte::CreateApplication();

int main(int argc, char** argv) 
{
	ForByte::Log::Init();
	FB_CORE_WARM("Initialize Log!");
	int a = 5;
	FB_INFO("Hello! Var={0}", a);

	auto app = ForByte::CreateApplication();
	app->Run();
	delete app;
}

#endif // FB_PLATFORM_WINDOWS
