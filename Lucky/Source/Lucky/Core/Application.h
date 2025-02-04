#pragma once

#include "Lucky/Core/Core.h"
#include "Lucky/Core/Window.h"
#include "Lucky/Core/LayerStack.h"
#include "Lucky/Core/DeltaTime.h"

#include "Lucky/Core/Events/Event.h"
#include "Lucky/Core/Events/ApplicationEvent.h"

#include "Lucky/ImGui/ImGuiLayer.h"

namespace Lucky
{
    /// <summary>
    /// Application 命令行参数
    /// </summary>
    struct ApplicationCommandLineArgs
    {
        int Count = 0;          // 参数个数
        char** Args = nullptr;  // 参数列表

        const char* operator[](int index) const
        {
            LC_CORE_ASSERT(index < Count, "Index 超过了参数数量！");

            return Args[index];
        }
    };

    /// <summary>
    /// Application 规范
    /// </summary>
    struct ApplicationSpecification
    {
        std::string Name = "Lucky Application";
        std::string WorkingDirectory;               // 项目工作目录

        ApplicationCommandLineArgs CommandLineArgs; // 命令行参数
    };

    class Application
    {
    private:
        static Application* s_Instance; // 静态实例

        ApplicationSpecification m_Specification;   // App 规范
        
        Scope<Window> m_Window;         // 窗口
        ImGuiLayer* m_ImGuiLayer;       // ImGui 层指针

        bool m_Running = true;          // 是否正在运行
        bool m_Minimized = false;       // 是否最小化

        LayerStack m_LayerStack;        // 层栈

        float m_LastFrameTime = 0.0f;   // 上一帧时间
        float m_FramesPerSecond = 0.0f; // 帧率
    public:
        Application(const ApplicationSpecification& specification);

        virtual ~Application();

        /// <summary>
        /// 添加普通层到层栈
        /// </summary>
        /// <param name="layer">普通层</param>
        void PushLayer(Layer* layer);

        /// <summary>
        /// 添加覆盖层到层栈
        /// </summary>
        /// <param name="layer">覆盖层</param>
        void PushOverlay(Layer* layer);

        /// <summary>
        /// 事件回调函数
        /// </summary>
        /// <param name="e">事件</param>
        void OnEvent(Event& e);

        /// <summary>
        /// 运行：主循环
        /// </summary>
        void Run();

        /// <summary>
        /// 关闭程序
        /// </summary>
        void Close();

        /// <summary>
        /// 返回帧率
        /// </summary>
        /// <returns>帧率</returns>
        inline float GetFPS() const { return m_FramesPerSecond; }

        /// <summary>
        /// 返回 Application 的实例
        /// </summary>
        /// <returns>实例</returns>
        inline static Application& GetInstance() { return *s_Instance; }

        /// <summary>
        /// 返回 Application 的窗口
        /// </summary>
        /// <returns></returns>
        inline Window& GetWindow() { return *m_Window; }

        /// <summary>
        /// 返回 ImGui 层
        /// </summary>
        /// <returns>ImGui 层</returns>
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        const ApplicationSpecification& GetSpecification() const { return m_Specification; }
    private:
        /// <summary>
        /// 窗口关闭回调函数
        /// </summary>
        /// <param name="e">窗口关闭事件</param>
        /// <returns>是否已关闭</returns>
        bool OnWindowClose(WindowCloseEvent& e);

        /// <summary>
        /// 窗口缩放时调用
        /// </summary>
        /// <param name="e">窗口缩放事件</param>
        /// <returns>事件处理结果</returns>
        bool OnWindowResize(WindowResizeEvent& e);
    };

    /// <summary>
    /// 创建 App：在 LuckyEditor 中定义
    /// </summary>
    /// <param name="args">命令行参数</param>
    /// <returns></returns>
    Application* CreateApplication(ApplicationCommandLineArgs args);
}