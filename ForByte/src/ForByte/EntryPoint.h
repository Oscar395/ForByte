#pragma once

#ifdef FB_PLATFORM_WINDOWS

extern ForByte::Application* ForByte::CreateApplication();

int main(int argc, char** argv) 
{
	auto app = ForByte::CreateApplication();
	app->Run();
	delete app;
}

#endif // FB_PLATFORM_WINDOWS
