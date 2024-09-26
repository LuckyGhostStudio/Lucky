#include "lcpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>

namespace Lucky
{
	/// <summary>
	/// 2D 渲染器数据
	/// </summary>
	struct Renderer2DData
	{
		std::shared_ptr<VertexArray> QuadVertexArray;	// 四边形顶点数组
		std::shared_ptr<Shader>	FlatColorShader;		// 四边形着色器
		std::shared_ptr<Shader> TextureShader;			// 纹理着色器
	};

	static Renderer2DData* s_Data;	// 渲染器数据

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData();

		s_Data->QuadVertexArray = std::make_shared<VertexArray>();		// 创建顶点数组对象

		float squareVertices[] = {
			//------位置------  --纹理坐标--
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	// 左下 0
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	// 右下 1
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,	// 左上 2
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f	// 右上 3
		};

		std::shared_ptr<VertexBuffer> squareVB;										// VBO
		squareVB.reset(new VertexBuffer(squareVertices, sizeof(squareVertices)));	// 创建顶点缓冲

		//设置顶点缓冲区布局
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },	// 位置
			{ ShaderDataType::Float2, "a_Texture" },	// 纹理坐标
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);		// 添加 VBO 到 VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };	// 顶点索引

		std::shared_ptr<IndexBuffer> squareIB;														// EBO
		squareIB.reset(new IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	// 创建索引缓冲
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);											// 设置 EBO 到 VAO

		s_Data->FlatColorShader = std::make_shared<Shader>("Assets/Shaders/FlatColorShader");		// 创建着色器
		s_Data->TextureShader = std::make_shared<Shader>("Assets/Shaders/TextureShader");			// 创建 Texture 着色器

		s_Data->TextureShader->Bind();					// 绑定 Texture 着色器
		s_Data->TextureShader->SetInt("u_Texture", 0);	// 设置 texture 变量
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;	// 释放渲染器数据
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->FlatColorShader->Bind();	// 绑定着色器
		s_Data->FlatColorShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	// 设置 vp 矩阵

		s_Data->TextureShader->Bind();		// 绑定 Texture 着色器
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());		// 设置 vp 矩阵
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();	// 绑定着色器

		s_Data->FlatColorShader->SetFloat4("u_Color", color);	// 设置 color

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		s_Data->FlatColorShader->SetMat4("u_Transform", transform);		// 设置 transform 矩阵

		s_Data->QuadVertexArray->Bind();						// 绑定顶点数组

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);	// 绘制
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();						// 绑定着色器
		s_Data->TextureShader->SetFloat4("u_Color", color);	// 设置 color

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		s_Data->TextureShader->SetMat4("u_Transform", transform);		// 设置 transform 矩阵

		texture->Bind();	// 绑定纹理槽

		s_Data->QuadVertexArray->Bind();						// 绑定顶点数组
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);	// 绘制
	}
}