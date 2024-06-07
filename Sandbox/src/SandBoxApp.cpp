#include <ForByte.h>
#include <ForByte/Core/EntryPoint.h>

#include "Sandbox2D.h"

class SandBox : public ForByte::Application {
public:
	SandBox() 
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~SandBox() 
	{

	}
};

ForByte::Application* ForByte::CreateApplication() 
{
	return new SandBox();
}