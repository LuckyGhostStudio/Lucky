#pragma once

#include <Lucky.h>

namespace Lucky
{
    class EditorLayer : public Layer
    {
    private:
        ShaderLibrary m_ShaderLibrary;                      // ��ɫ����

        CameraController m_CameraController;                // ���������

        std::shared_ptr<Texture2D> m_CheckerboardTexture;   // ��������

        std::shared_ptr<Framebuffer> m_Framebuffer;         // ֡������

        bool m_ViewportFocused = false;     // �ӿڱ��۽�
        bool m_ViewportHovered = false;     // �����ͣ���ӿ�

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };          // �ӿڴ�С

        glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_SquareRotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 m_SquareScale = { 1.0f, 1.0f, 1.0f };

        glm::vec4 m_SquareColor = { 0.2f,0.3f,0.8f, 1.0f };

        float fps = 0.0f;
    public:
        EditorLayer();

        virtual ~EditorLayer() = default;

        /// <summary>
        /// ���ò����ӵ���ջʱ����
        /// </summary>
        virtual void OnAttach() override;

        /// <summary>
        /// ���ò�Ӳ�ջ�Ƴ�ʱ����
        /// </summary>
        virtual void OnDetach() override;

        /// <summary>
        /// ���£�ÿ֡����
        /// </summary>
        /// <param name="dt">֡���</param>
        virtual void OnUpdate(DeltaTime dt) override;

        /// <summary>
        /// ��Ⱦ ImGui
        /// </summary>
        virtual void OnImGuiRender() override;

        /// <summary>
        /// �¼���������
        /// </summary>
        /// <param name="event">�¼�</param>
        virtual void OnEvent(Event& event) override;
    };
}