#pragma once
#include "entt.hpp"


namespace ForByte {

	class Scene {
		Scene();
		~Scene();
	private:
		entt::registry m_Registry;
	};

}