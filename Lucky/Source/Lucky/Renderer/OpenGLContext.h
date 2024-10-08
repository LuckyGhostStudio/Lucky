#pragma once

struct GLFWwindow;

namespace Lucky
{
    /// <summary>
    /// OpenGL 上下文
    /// </summary>
    class OpenGLContext
    {
    private:
        GLFWwindow* m_WindowHandle;     // GLFW 窗口句柄

    public:
        /// <summary>
        /// OpenGL 上下文
        /// </summary>
        /// <param name="windowHandle">窗口句柄</param>
        OpenGLContext(GLFWwindow* windowHandle);

        /// <summary>
        /// 初始化
        /// </summary>
        void Init();

        /// <summary>
        /// 交换前后缓冲
        /// </summary>
        void SwapBuffers();
    };
}