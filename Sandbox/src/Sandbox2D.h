#pragma once

#include "ForByte.h"

#include "ParticleSystem.h"

class Sandbox2D : public ForByte::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(ForByte::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(ForByte::Event& e) override;
private:
	ForByte::OrthographicCameraController m_CameraController;

	//Temp
	ForByte::Ref<ForByte::VertexArray> m_SquareVA;
	ForByte::Ref<ForByte::Shader> m_FlatColorShader;

	ForByte::Ref<ForByte::Texture2D> m_CheckerboardTexture;
	ForByte::Ref<ForByte::Texture2D> m_SpriteSheet;
	ForByte::Ref<ForByte::SubTexture2D> m_TextureStairs, m_TextureBarrel, m_TextureTree;

	glm::vec4 m_SquareColor = { 0.1f, 0.3f, 0.8f , 1.0f};

	ParticleSystem m_ParticleSysten;
	ParticleProps m_Particle;

	uint32_t m_MapWidth, m_MapHeight;
	std::unordered_map<char, ForByte::Ref<ForByte::SubTexture2D>> m_TextureMap;
};