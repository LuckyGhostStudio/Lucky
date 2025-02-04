#pragma once

#include "Scene.h"

namespace Lucky
{
    /// <summary>
    /// 场景序列化器
    /// </summary>
    class SceneSerializer
    {
    private:
        Ref<Scene> m_Scene; //场景
    public:
        /// <summary>
        /// 场景序列化器
        /// </summary>
        /// <param name="scene">场景</param>
        SceneSerializer(const Ref<Scene> scene);

        /// <summary>
        /// 序列化场景：序列化为文本文件
        /// </summary>
        /// <param name="filepath">文件输出路径</param>
        void Serialize(const std::string& filepath);

        /// <summary>
        /// 运行时序列化场景：序列化为二进制
        /// </summary>
        /// <param name="filepath">文件输出路径</param>
        void SerializeRuntime(const std::string& filepath);

        /// <summary>
        /// 反序列化场景：文本文件反序列化
        /// </summary>
        /// <param name="filepath">文件路径</param>
        /// <returns>结果</returns>
        bool Deserialize(const std::string& filepath);

        /// <summary>
        /// 运行时反序列化：二进制反序列化
        /// </summary>
        /// <param name="filepath">文件路径</param>
        /// <returns>结果</returns>
        bool DeserializeRuntime(const std::string& filepath);
    };
}