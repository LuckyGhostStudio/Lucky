#pragma once

#include "Lucky/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Lucky
{
    /*
     * ��ͨ�����б�ǰ�벿�� ���ǲ����б���벿��
     * ���ǲ㣺GUI��Guizmo ��
     */

    /// <summary>
    /// ��ջ
    /// </summary>
    class LayerStack
    {
    private:
        std::vector<Layer*> m_Layers;           // ���б�
        unsigned int m_LayerInsertIndex = 0;    // ����λ������
    public:
        LayerStack();
        ~LayerStack();

        /// <summary>
        /// ��layer���ӵ� ��������� ��λ��
        /// </summary>
        /// <param name="layer">��</param>
        void PushLayer(Layer* layer);

        /// <summary>
        /// �����ǲ����ӵ�ջ��
        /// </summary>
        /// <param name="overlay">���ǲ�</param>
        void PushOverlay(Layer* overlay);

        /// <summary>
        /// ��layer�Ƴ���ջ
        /// </summary>
        /// <param name="layer">��ͨ��</param>
        void PopLayer(Layer* layer);

        /// <summary>
        /// �����ǲ��Ƴ���ջ
        /// </summary>
        /// <param name="overlay">���ǲ�</param>
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }
    };
}