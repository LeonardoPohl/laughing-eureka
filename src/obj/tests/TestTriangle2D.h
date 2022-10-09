#pragma once

#include <GLCore/GLObject.h>

#include <GLCore/Renderer.h>
#include <GLCore/Texture.h>
#include <GLCore/VertexBuffer.h>
#include <GLCore/VertexBufferLayout.h>

#include <memory>

namespace GLObject
{
	class TestTriangle2D : public GLObject
	{
	public:
		TestTriangle2D();

		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_Color[4];

		glm::vec2 m_LeftRightOrtho;
		glm::vec2 m_BottomTopOrtho;
		glm::vec2 m_NearFarOrtho;

		glm::vec3 m_ModelTranslation;
		glm::vec3 m_ViewTranslation;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VB;
		std::unique_ptr<VertexBufferLayout> m_VBL;

		glm::mat4 m_View;
		glm::mat4 m_Proj;
	};
}