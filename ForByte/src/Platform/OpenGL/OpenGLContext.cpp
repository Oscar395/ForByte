#include "fbpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace ForByte {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		FB_CORE_ASSERT(windowHandle, "Window handle is null!")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FB_CORE_ASSERT(status, "Failed to initialize Glad!");

		FB_CORE_INFO("OpenGL Info:");
		FB_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		FB_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		FB_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));

	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}