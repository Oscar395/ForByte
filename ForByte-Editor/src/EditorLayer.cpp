#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ForByte/Scene/SceneSerializer.h>

#include "ForByte/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "ForByte/Math/Math.h"

namespace ForByte {

	extern const std::filesystem::path g_Assetspath;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		FB_PROFILE_FUNCTION();
		//m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		moveIconTexture = Texture2D::Create("Resources/Icons/Viewport/moveIcon.png");
		rotateIconTexture = Texture2D::Create("Resources/Icons/Viewport/rotateIcon.png");
		scaleIconTexture = Texture2D::Create("Resources/Icons/Viewport/scaleIcon.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0

		auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;
			}

			void OnUpdate(Timestep ts)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;
				
				if (Input::IsKeyPressed(FB_KEY_A))
					translation.x -= speed * ts;

				if (Input::IsKeyPressed(FB_KEY_D))
					translation.x += speed * ts;

				if (Input::IsKeyPressed(FB_KEY_W))
					translation.y += speed * ts;

				if (Input::IsKeyPressed(FB_KEY_S))
					translation.y -= speed * ts;
			}

			void OnDestroy()
			{

			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
		FB_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		FB_PROFILE_FUNCTION();

		//m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Update
		if (m_ViewportFocused)
			m_CameraController.OnUpdate(ts);

		m_EditorCamera.OnUpdate(ts);

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		//Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

		auto[mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
		}

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		FB_PROFILE_FUNCTION();
		/*static bool show = true;
		ImGui::ShowDemoWindow(&show);*/

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// ecause it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);


		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 200.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.

				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveScene(m_SceneFilePath);

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem("Exit"))
					ForByte::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		std::string name = "None";

		if (m_HoveredEntity && m_HoveredEntity.HasComponent<TagComponent>())
			name = m_HoveredEntity.GetComponent<TagComponent>().Tag;

		ImGui::Text("Hovered Entity: %s", name.c_str());

		auto stats = ForByte::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("DrawCalls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		//auto viewportOffset = ImGui::GetCursorPos(); // Includes tab bar

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 ViewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*)&ViewportPanelSize) && ViewportPanelSize.x > 0 && ViewportPanelSize.y > 0) 
		{
			m_Framebuffer->Resize((uint32_t)ViewportPanelSize.x, (uint32_t)ViewportPanelSize.y);
			m_ViewportSize = { ViewportPanelSize.x, ViewportPanelSize.y };
			m_EditorCamera.SetViewportSize(ViewportPanelSize.x, ViewportPanelSize.y);
			m_ActiveScene->OnViewportResize(ViewportPanelSize.x, ViewportPanelSize.y);
			//m_CameraController.OnResize(ViewportPanelSize.x, ViewportPanelSize.y);
		}

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_Assetspath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Camera
			 
			// Runtime camera from entity
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Editor Camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			bool snap = Input::IsKeyPressed(FB_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				//glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation = rotation;
				tc.Scale = scale;
			}
		}

		// viewport gizmos control
		float padding = 8.0f;
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		vMin.x += ImGui::GetWindowPos().x + padding;
		vMin.y += ImGui::GetWindowPos().y + padding;

		ImGui::SetNextWindowPos(vMin);

		ImGuiWindowFlags window_sflags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

		ImVec2 iconSize = ImVec2(28.0f, 28.0f);
		//ImVec4 bgButtonColor = ImVec4(2, 2, 0, 0);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::Begin("controls", 0, window_sflags))
		{
			if (ImGui::ImageButton((ImTextureID)moveIconTexture->GetRendererID(), iconSize, { 0, 1 }, { 1, 0 }))
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

			if (ImGui::ImageButton((ImTextureID)rotateIconTexture->GetRendererID(), iconSize, { 0, 1 }, { 1, 0 }))
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;

			if (ImGui::ImageButton((ImTextureID)scaleIconTexture->GetRendererID(), iconSize, { 0, 1 }, { 1, 0 }))
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
		}
		ImGui::PopStyleColor();

		ImGui::End();

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Distpatch<KeyPressedEvent>(FB_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Distpatch<MouseButtonPressedEvent>(FB_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(FB_KEY_LEFT_CONTROL) || Input::IsKeyPressed(FB_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(FB_KEY_LEFT_SHIFT) || Input::IsKeyPressed(FB_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
			case FB_KEY_N:
			{
				if (control)
					NewScene();
				break;
			}

			case FB_KEY_O:
			{
				if (control)
					OpenScene();
				break;
			}

			case FB_KEY_S:
			{
				if (control && shift)
					SaveSceneAs();

				if (control)
					SaveScene(m_SceneFilePath);
				break;
			}

			// Gizmos
			case FB_KEY_Q:
				m_GizmoType = -1;
				break;
			case FB_KEY_W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case FB_KEY_E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case FB_KEY_R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == FB_MOUSE_BUTTON_LEFT)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(FB_KEY_LEFT_ALT) && !Input::IsKeyPressed(FB_KEY_LEFT_SHIFT))
			{
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("ForByte Scene (*.fb)\0*.fb\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.Deserialize(path.string());
		m_SceneFilePath = path.string();
	}

	void EditorLayer::SaveScene(std::string& sceneFilepath)
	{
		if (!sceneFilepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(sceneFilepath);
			m_SceneFilePath = sceneFilepath;
		}
		else {
			SaveSceneAs();
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("ForByte Scene (*.fb)\0*.fb\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
			m_SceneFilePath = filepath;
		}
	}
}