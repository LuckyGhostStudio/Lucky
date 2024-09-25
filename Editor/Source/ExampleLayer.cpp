#include "ExampleLayer.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_CameraController(1280.0f / 720.0f)
{
	
}

void ExampleLayer::OnAttach()
{
	m_VertexArray.reset(new Lucky::VertexArray());		// ���������������

	float vertices[] = {
		// ----- λ�� -----  -------- ��ɫ --------
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	// ����
		 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	// ����
		 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	// ��
	};

	std::shared_ptr<Lucky::VertexBuffer> vertexBuffer;								// VBO
	vertexBuffer.reset(new Lucky::VertexBuffer(vertices, sizeof(vertices)));		// �������㻺����

	// ���㻺��������
	Lucky::BufferLayout layout = {
		{ Lucky::ShaderDataType::Float3, "a_Position" },	// λ��
		{ Lucky::ShaderDataType::Float4, "a_Color" }		// ��ɫ
	};

	vertexBuffer->SetLayout(layout);				// ���ö��㻺��������
	m_VertexArray->AddVertexBuffer(vertexBuffer);	// ���� VBO �� VAO

	unsigned int indices[3] = { 0, 1, 2 };			// ��������

	std::shared_ptr<Lucky::IndexBuffer> indexBuffer;										// EBO
	indexBuffer.reset(new Lucky::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));	// ������������
	m_VertexArray->SetIndexBuffer(indexBuffer);												// ���� EBO �� VAO

	m_Shader.reset(new Lucky::Shader("Assets/Shaders/TriangleShader"));	// ������ɫ��

	//Square
	m_SquareVA.reset(new Lucky::VertexArray());		// ���������������

	float squareVertices[] = {
		//------λ��------  --��������--
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,	// ����0
		 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,	// ����1
		-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,	// ����2
		 0.5f,  0.5f, 0.0f,	1.0f, 1.0f	// ����3
	};

	std::shared_ptr<Lucky::VertexBuffer> squareVB;										// VBO

	squareVB.reset(new Lucky::VertexBuffer(squareVertices, sizeof(squareVertices)));	// �������㻺��

	squareVB->SetLayout({
		{ Lucky::ShaderDataType::Float3, "a_Position" },	// λ��
		{ Lucky::ShaderDataType::Float2, "a_TexCoord" },	// ��������
		});

	m_SquareVA->AddVertexBuffer(squareVB);					// ���� VBO �� VAO

	unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };	// ��������

	std::shared_ptr<Lucky::IndexBuffer> squareIB;														// EBO
	squareIB.reset(new Lucky::IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	// ������������
	m_SquareVA->SetIndexBuffer(squareIB);																// ���� EBO �� VAO


	m_FlatColorShader.reset(new Lucky::Shader("Assets/Shaders/FlatColorShader"));	// ������ɫ��

	auto textureShader = m_ShaderLibrary.Load("Assets/Shaders/TextureShader");		// ������ɫ��

	m_Texture.reset(new Lucky::Texture2D("Assets/Textures/Checkerboard.png"));				// ��������
	m_LuckyLogoTexture.reset(new Lucky::Texture2D("Assets/Textures/LuckyLogoBlue.png"));	// ��������

	textureShader->Bind();
	textureShader->UploadUniformInt("u_Texture", 0);
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Lucky::DeltaTime dt)
{
	LC_TRACE("FPS: {0}", (int)(1000.0f / dt.GetMilliseconds()));



	Lucky::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Lucky::RenderCommand::Clear();

	m_CameraController.OnUpdate(dt);	// �������������

	Lucky::Renderer::BeginScene(m_CameraController.GetCamera());			// ��ʼ��Ⱦ����

	static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	m_FlatColorShader->Bind();
	m_FlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 20; x++) {
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;		// �����εı任����

			Lucky::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);		// �ύ��Ⱦָ��
		}
	}

	auto textureShader = m_ShaderLibrary.Get("TextureShader");

	m_Texture->Bind();
	Lucky::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_LuckyLogoTexture->Bind();
	Lucky::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


	Lucky::Renderer::EndScene();						// ������Ⱦ����
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void ExampleLayer::OnEvent(Lucky::Event& event)
{
	m_CameraController.OnEvent(event);		// ��������¼�����
}