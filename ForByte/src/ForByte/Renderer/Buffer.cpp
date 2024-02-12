#include "fbpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace ForByte {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:   FB_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		FB_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:   FB_CORE_ERROR("RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		FB_CORE_ERROR("Unknown RendererAPI!");
		return nullptr;
	}
}