#include "lcpch.h"
#include "Application.h"

#include "Lucky/Renderer/Renderer.h"
#include "Lucky/Script/ScriptEngine.h"

#include <filesystem>

#include <GLFW/glfw3.h>

namespace Lucky
{
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecification& specification)
        : m_Specification(specification)
    {
        LC_CORE_ASSERT(!s_Instance, "Application already exists!");

        s_Instance = this;

        // 设置项目当前工作目录
        if (!m_Specification.WorkingDirectory.empty())
        {
            std::filesystem::current_path(m_Specification.WorkingDirectory);
        }

        LC_CORE_INFO("Working Directory: {0}", std::filesystem::current_path().string());

        m_Window = Window::Create(WindowProps(m_Specification.Name));           // 创建窗口
        m_Window->SetEventCallback(LC_BIND_EVENT_FUNC(Application::OnEvent));   // 设置回调函数

        Renderer::Init();       // 初始化渲染器
        ScriptEngine::Init();   // 初始化脚本引擎

        m_ImGuiLayer = new ImGuiLayer();    // 创建 ImGui 层
        PushOverlay(m_ImGuiLayer);          // 添加 ImGuiLayer 到覆盖层
    }

    Application::~Application()
    {
        ScriptEngine::Shutdown();
        Renderer::Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);  // 事件调度器
        dispatcher.Dispatch<WindowCloseEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowClose));      // 调度 窗口关闭事件
        dispatcher.Dispatch<WindowResizeEvent>(LC_BIND_EVENT_FUNC(Application::OnWindowResize));    // 调度 窗口缩放事件

        // 从最顶层向下遍历层栈
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            if (e.GetHandled())
            {
                break;  // 事件已处理
            }
            (*--it)->OnEvent(e);    // 层获取并处理事件
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            float time = (float)glfwGetTime();              // 当前时间
            DeltaTime deltaTime = time - m_LastFrameTime;   // 帧间隔 = 当前时间 - 上一帧时间
            m_LastFrameTime = time;                         // 更新上一帧时间

            m_FramesPerSecond = 1.0f / deltaTime;           // 帧率 TODO 计算 1s 内平均帧率

            // 窗口未最小化
            if (!m_Minimized)
            {
                // 更新层栈中所有层
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnUpdate(deltaTime);
                }

                // ImGui渲染
                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                {
                    layer->OnImGuiRender(); // 渲染每个 Layer 的 ImGui
                }
                m_ImGuiLayer->End();
            }

            m_Window->OnUpdate();           // 更新窗口
        }
    }

    void Application::Close()
    {
        m_Running = false;  // 结束运行
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;  // 结束运行
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;        // 窗口最小化
            return false;
        }

        m_Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());  //进行窗口缩放后的设置
        // TODO 更新 ImGui 的 DisplaySize

        return false;
    }
}