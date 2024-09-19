#include "lcpch.h"
#include "Input.h"

#include "Lucky/Application.h"

#include <GLFW/glfw3.h>

namespace Lucky
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	// GLFW����
		auto state = glfwGetKey(window, (int)keycode);			// ��ȡkeycode����״̬
		return state == GLFW_PRESS || state == GLFW_REPEAT;		// ��������||�����ظ�
	}
	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, (int)button);	// ��ȡbutton״̬
		return state == GLFW_PRESS;								// ��������
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);		// ��ȡ��� x y ����

		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().first;	// x����
	}
	float Input::GetMouseY()
	{
		return GetMousePosition().second;	// y����
	}
}