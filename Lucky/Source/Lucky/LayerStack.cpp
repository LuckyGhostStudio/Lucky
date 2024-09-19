#include "lcpch.h"
#include "LayerStack.h"

namespace Lucky
{
	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		// ɾ�����в�
		for (Layer* layer : m_Layers) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);		// ����layer�� m_LayerInsertIndexλ��
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);	// ���� overlay �� ջ��
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);		// ����layer

		// �ҵ�layer
		if (it != m_Layers.end()) {
			layer->OnDetach();
			m_Layers.erase(it);			// �Ƴ�itָ���layer
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);	// ����overlay

		if (it != m_Layers.end()) {
			overlay->OnDetach();
			m_Layers.erase(it);		// �Ƴ�itָ���overlay
		}
	}
}