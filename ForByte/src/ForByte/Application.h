#pragma once

#include "Core.h"

#include "Window.h"
#include "ForByte/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ForByte/ImGui/ImGuiLayer.h"

namespace ForByte {

	class FORBYTE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	//to be defined in the client
	Application* CreateApplication();
}
