#include <ForByte.h>

class ExampleLayer : public ForByte::Layer
{
public:
	ExampleLayer() 
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		FB_INFO("ExampleLayer::Update");
	}

	void OnEvent(ForByte::Event& event) override
	{
		FB_TRACE("{0}", event);
	}
};

class SandBox : public ForByte::Application {
public:
	SandBox() 
	{
		PushLayer(new ExampleLayer());

	}
	~SandBox() 
	{

	}
};

ForByte::Application* ForByte::CreateApplication() 
{
	return new SandBox();
}