#pragma once

#include "ForByte.h"

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
	ForByte::Ref<ForByte::VertexArray> m_SquareVA;
	ForByte::Ref<ForByte::Shader> m_FlatColorShader;
	glm::vec4 m_SquareColor = { 0.1f, 0.3f, 0.8f , 1.0f};
};