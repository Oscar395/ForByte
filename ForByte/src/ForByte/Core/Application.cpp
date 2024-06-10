#include "fbpch.h"
#include "Application.h"

#include "Log.h"

#include "ForByte/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace ForByte {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		FB_PROFILE_FUNCTION();

		FB_CORE_ASSERT(!s_Instance, "Application already exits!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer)
	{
		FB_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		FB_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e) 
	{
		FB_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Distpatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Distpatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) 
		{
			if (e.m_Handled)
				break;
			(*--it)->OnEvent(e);
		}
	}

	void Application::Run() {
		FB_PROFILE_FUNCTION();

		while (m_Running)
		{
			FB_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					FB_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
				m_ImGuiLayer->Begin();
				{
					FB_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		FB_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}