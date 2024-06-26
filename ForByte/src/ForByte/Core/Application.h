#pragma once

#include "Core.h"

#include "Window.h"
#include "ForByte/Core/LayerStack.h"
#include "ForByte/Events/Event.h"
#include "ForByte/Events/ApplicationEvent.h"

#include "ForByte/ImGui/ImGuiLayer.h"

#include "ForByte/Core/Timestep.h"

namespace ForByte {

	class Application
	{
	public:
		Application(const std::string& name = "ForByte App");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	//to be defined in the client
	Application* CreateApplication();
}
