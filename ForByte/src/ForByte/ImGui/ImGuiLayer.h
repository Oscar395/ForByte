#pragma once

#include "ForByte/Core/Layer.h"

#include "ForByte/Events/ApplicationEvent.h"
#include "ForByte/Events/KeyEvent.h"
#include "ForByte/Events/MouseEvent.h"

namespace ForByte {

	class FORBYTE_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}