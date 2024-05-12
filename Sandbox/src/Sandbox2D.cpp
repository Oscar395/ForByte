#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	FB_PROFILE_FUNCTION();
	m_CheckerboardTexture = ForByte::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	FB_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(ForByte::Timestep ts)
{
	FB_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	{
		FB_PROFILE_SCOPE("Renderer Prep");
		ForByte::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		ForByte::RenderCommand::Clear();
	}

	{
		FB_PROFILE_SCOPE("Renderer Draw");
		ForByte::Renderer2D::BeginScene(m_CameraController.GetCamera());
		ForByte::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), {0.8f, 0.2f, 0.3f, 1.0f});
		ForByte::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		ForByte::Renderer2D::DrawQuad({ 0.0f, 0.0f , -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture, 10.0f, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f));
		ForByte::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	FB_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(ForByte::Event& e)
{
	m_CameraController.OnEvent(e);
}