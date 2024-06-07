#include <ForByte.h>
#include <ForByte/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace ForByte {

	class ForByteEditor : public Application {
	public:
		ForByteEditor()
			: Application("ForByte Editor")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}
		~ForByteEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new ForByteEditor();
	}
}