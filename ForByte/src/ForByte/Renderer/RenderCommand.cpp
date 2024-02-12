#include "fbpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ForByte {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}