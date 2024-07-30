#pragma once

#include "ForByte/Core/Core.h"
#include "ForByte/Core/Log.h"
#include "ForByte/Scene/Scene.h"
#include "ForByte/Scene/Entity.h"

namespace ForByte {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(Ref<Scene>& scene);

		void SetContext(Ref<Scene>& scene);

		void OnImGuiRender();
		
		Entity GetSelectedEntity() const { return m_SelectionContext; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}