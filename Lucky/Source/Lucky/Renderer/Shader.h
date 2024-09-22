#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Lucky
{
	/// <summary>
	/// 着色器
	/// </summary>
	class Shader
	{
	private:
		uint32_t m_RendererID;	// 着色器 ID

		/// <summary>
		/// 读文件
		/// </summary>
		/// <param name="filepath">文件路径</param>
		/// <returns>文件内容</returns>
		std::string ReadFile(const std::string& filepath);

		/// <summary>
		/// 编译着色器
		/// </summary>
		/// <param name="shaderSources">着色器类型 - 着色器源码 map</param>
		void Compile(std::unordered_map<unsigned int, std::string>& shaderSources);
	public:
		/// <summary>
		/// 着色器
		/// </summary>
		/// <param name="vertexShaderPath">顶点着色器文件路径</param>
		/// <param name="fragmentShaderPath">片段着色器文件路径</param>
		Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		
		~Shader();

		/// <summary>
		/// 绑定：使用着色器
		/// </summary>
		void Bind() const;

		/// <summary>
		/// 解除绑定：调试时使用
		/// </summary>
		void UnBind() const;

		// ---- 下列方法：上传 Uniform 变量到 Shader ---- |（变量在 Shader 中的变量名，变量值）

		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};
}