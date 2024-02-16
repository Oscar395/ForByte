#include <ForByte.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public ForByte::Layer
{
public:
	ExampleLayer() 
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(ForByte::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.1f, 0.8f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.2f, 0.8f, 1.0f,
			 0.0f, 0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<ForByte::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(ForByte::VertexBuffer::Create(vertices, sizeof(vertices)));

		ForByte::BufferLayout layout = {
			{ ForByte::ShaderDataType::Float3, "a_Position" },
			{ ForByte::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<ForByte::IndexBuffer> indexBuffer;
		indexBuffer.reset(ForByte::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		m_SquareVA.reset(ForByte::VertexArray::Create());
		std::shared_ptr<ForByte::VertexBuffer> squareVB;
		squareVB.reset(ForByte::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ForByte::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<ForByte::IndexBuffer> squareIB;
		squareIB.reset(ForByte::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
        )";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
        )";

		m_Shader.reset(new ForByte::Shader(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
        )";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec4 u_Color;

			void main()
			{
				color = u_Color;
			}
        )";

		m_FlatColorShader.reset(new ForByte::Shader(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));
	}

	void OnUpdate(ForByte::Timestep ts) override
	{
		if (ForByte::Input::IsKeyPressed(FB_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (ForByte::Input::IsKeyPressed(FB_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (ForByte::Input::IsKeyPressed(FB_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (ForByte::Input::IsKeyPressed(FB_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (ForByte::Input::IsKeyPressed(FB_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (ForByte::Input::IsKeyPressed(FB_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		ForByte::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		ForByte::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		ForByte::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if (x % 2 == 0) {
					m_FlatColorShader->UpdloadUniformFloat4("u_Color", secondColor);
				}
				else
				{
					m_FlatColorShader->UpdloadUniformFloat4("u_Color", firstColor);
				}
				ForByte::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
	
		ForByte::Renderer::Submit(m_Shader, m_VertexArray);

		ForByte::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Color changer");

		ImGui::ColorEdit4("First Color", (float*)&firstColor);
		ImGui::ColorEdit4("Second Color", (float*)&secondColor);

		ImGui::End();
	}

	void OnEvent(ForByte::Event& event) override
	{
	}

	bool OnKeyPressedEvent(ForByte::KeyPressedEvent& event)
	{
	}
private:
	std::shared_ptr<ForByte::Shader> m_Shader;
	std::shared_ptr<ForByte::VertexArray> m_VertexArray;

	std::shared_ptr<ForByte::Shader> m_FlatColorShader;
	std::shared_ptr<ForByte::VertexArray> m_SquareVA;

	ForByte::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec4 firstColor = glm::vec4(1.0f);
	glm::vec4 secondColor = glm::vec4(1.0f);
};

class SandBox : public ForByte::Application {
public:
	SandBox() 
	{
		PushLayer(new ExampleLayer());
	}
	~SandBox() 
	{

	}
};

ForByte::Application* ForByte::CreateApplication() 
{
	return new SandBox();
}