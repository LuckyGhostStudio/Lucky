#include "lcpch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace Lucky::Math
{
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
    {
        // From glm::decompose in matrix_decompose.inl

        using namespace glm;

        mat4 LocalMatrix(transform);

        // Normalize the matrix.
        if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<float>()))
            return false;

        // First, isolate perspective.  This is the messiest.
        if (
            epsilonNotEqual(LocalMatrix[0][3], static_cast<float>(0), epsilon<float>()) ||
            epsilonNotEqual(LocalMatrix[1][3], static_cast<float>(0), epsilon<float>()) ||
            epsilonNotEqual(LocalMatrix[2][3], static_cast<float>(0), epsilon<float>()))
        {
            // Clear the perspective partition
            LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<float>(0);
            LocalMatrix[3][3] = static_cast<float>(1);
        }

        // Next take care of translation (easy).
        position = vec3(LocalMatrix[3]);
        LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

        vec3 Row[3], Pdum3;

        // Now get scale and shear.
        for (length_t i = 0; i < 3; ++i)
        {
            for (length_t j = 0; j < 3; ++j)
            {
                Row[i][j] = LocalMatrix[i][j];
            }
        }

        // Compute X scale factor and normalize first row.
        scale.x = length(Row[0]);
        Row[0] = detail::scale(Row[0], static_cast<float>(1));
        scale.y = length(Row[1]);
        Row[1] = detail::scale(Row[1], static_cast<float>(1));
        scale.z = length(Row[2]);
        Row[2] = detail::scale(Row[2], static_cast<float>(1));

        rotation.y = asin(-Row[0][2]);
        if (cos(rotation.y) != 0)
        {
            rotation.x = atan2(Row[1][2], Row[2][2]);
            rotation.z = atan2(Row[0][1], Row[0][0]);
        }
        else
        {
            rotation.x = atan2(-Row[2][0], Row[1][1]);
            rotation.z = 0;
        }
        rotation = glm::degrees(rotation);

        return true;
    }
}