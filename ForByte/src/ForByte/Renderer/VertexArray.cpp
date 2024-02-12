#include "fbpch.h"

#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ForByte {

	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:   FB_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}

		FB_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}