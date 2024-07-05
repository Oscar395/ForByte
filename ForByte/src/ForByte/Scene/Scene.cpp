#include "fbpch.h"
#include "Scene.h"

#include "glm/glm.hpp"

namespace ForByte {

	static void DoMath(const glm::mat4& transform)
	{

	}

	Scene::Scene() 
	{

		struct MeshComponent
		{

		};
		struct TransformComponent 
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4 transform) :
				Transform(transform) {}

			operator glm::mat4&() { return Transform; }
			operator const glm::mat4&() const { return Transform; }
		};

		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		TransformComponent& tr = m_Registry.get<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			 auto& transform = group.get<TransformComponent, MeshComponent>(entity);
		}
	}

	Scene::~Scene()
	{

	}

}