#include "lcpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Lucky
{
    SceneCamera::SceneCamera()
    {
        RecalculateProjection();    // 重新计算投影矩阵
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Orthographic;    // 正交投影

        m_Size = size;
        m_Near = nearClip;
        m_Far = farClip;

        RecalculateProjection();    // 重新计算投影矩阵
    }

    void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Perspective;     //透视投影

        m_Fov = glm::radians(fov);
        m_Near = nearClip;
        m_Far = farClip;

        RecalculateProjection();    // 重新计算投影矩阵
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        LC_CORE_ASSERT(width > 0 && height > 0, "width or height <= 0!");

        m_AspectRatio = (float)width / (float)height;   // 宽高比

        RecalculateProjection();    // 重新计算投影矩阵
    }

    void SceneCamera::RecalculateProjection()
    {
        if (m_ProjectionType == ProjectionType::Perspective)    // 透视投影
        {
            m_ProjectionMatrix = glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far); // 设置透视投影矩阵
        }
        else
        {
            float orthLeft = -m_Size * m_AspectRatio;   // 左边界
            float orthRight = m_Size * m_AspectRatio;   // 右边界
            float orthBottom = -m_Size;                 // 下边界
            float orthTop = m_Size;                     // 上边界

            m_ProjectionMatrix = glm::ortho(orthLeft, orthRight, orthBottom, orthTop, m_Near, m_Far);   // 设置正交投影矩阵
        }
    }
}