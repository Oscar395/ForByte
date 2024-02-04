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
		if (ForByte::Input::IsKeyPressed(FB_KEY_TAB)) {
			FB_TRACE("Tab key is pressed (poll)!");
		}
	}

	void OnEvent(ForByte::Event& event) override
	{
		// FB_TRACE("{0}", event);
		if (event.GetEventType() == ForByte::EventType::KeyPressed)
		{
			ForByte::KeyPressedEvent& e = (ForByte::KeyPressedEvent&)event;
			if (e.GetKeyCode() == FB_KEY_TAB)
				FB_TRACE("Tab key is pressed (event)!");
			FB_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class SandBox : public ForByte::Application {
public:
	SandBox() 
	{
		PushLayer(new ExampleLayer());
		PushLayer(new ForByte::ImGuiLayer());
	}
	~SandBox() 
	{

	}
};

ForByte::Application* ForByte::CreateApplication() 
{
	return new SandBox();
}