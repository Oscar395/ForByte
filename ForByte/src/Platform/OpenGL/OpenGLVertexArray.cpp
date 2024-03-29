#include "fbpch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace ForByte {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ForByte::ShaderDataType::Float:  return GL_FLOAT;
			case ForByte::ShaderDataType::Float2: return GL_FLOAT;
			case ForByte::ShaderDataType::Float3: return GL_FLOAT;
			case ForByte::ShaderDataType::Float4: return GL_FLOAT;
			case ForByte::ShaderDataType::Mat3:   return GL_FLOAT;
			case ForByte::ShaderDataType::Mat4:   return GL_FLOAT;
			case ForByte::ShaderDataType::Int:    return GL_INT;
			case ForByte::ShaderDataType::Int2:   return GL_INT;
			case ForByte::ShaderDataType::Int3:   return GL_INT;
			case ForByte::ShaderDataType::Int4:   return GL_INT;
			case ForByte::ShaderDataType::Bool:   return GL_BOOL;
		}

		FB_CORE_ERROR("Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() 
	{
		glGenVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		FB_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has not layout!");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}