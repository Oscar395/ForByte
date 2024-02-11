#pragma once

#include "Core.h"

#include "Window.h"
#include "ForByte/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "ForByte/ImGui/ImGuiLayer.h"

#include "ForByte/Renderer/Shader.h"
#include "ForByte/Renderer/Buffer.h"
#include "ForByte/Renderer/VertexArray.h"

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

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;
	private:
		static Application* s_Instance;
	};

	//to be defined in the client
	Application* CreateApplication();
}
