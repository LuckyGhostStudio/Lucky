#pragma once

#include "Lucky/Core.h"
#include "Lucky/Events/Event.h"

namespace Lucky
{
	class Layer
	{
	protected:
		std::string m_DebugName;	// Layer在Debug模式中的名字
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		/// <summary>
		/// 将该层添加到层栈时调用
		/// </summary>
		virtual void OnAttach() {}

		/// <summary>
		/// 将该层从层栈移除时调用
		/// </summary>
		virtual void OnDetach() {}

		/// <summary>
		/// 该层更新时调用：每帧调用
		/// </summary>
		virtual void OnUpdate() {}

		/// <summary>
		/// 渲染ImGui
		/// </summary>
		virtual void OnImGuiRender() {}

		/// <summary>
		/// 事件函数：接收事件
		/// </summary>
		/// <param name="event">事件</param>
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};
}