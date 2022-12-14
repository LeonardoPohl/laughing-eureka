#include "TestTexture2D.h"

#include <GLCore/GLErrorManager.h>
#include <imgui.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace GLObject
{
    TestTexture2D::TestTexture2D(const Camera *cam)
    {
        this->camera = cam;
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,  // 0
             50.0f, -50.0f, 1.0f, 0.0f,  // 1
             50.0f,  50.0f, 1.0f, 1.0f,  // 2
            -50.0f,  50.0f, 0.0f, 1.0f,  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();

        m_VB = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        m_VBL = std::make_unique<VertexBufferLayout>();

        m_VBL->Push<float>(2);
        m_VBL->Push<float>(2);

        m_VAO->AddBuffer(*m_VB, *m_VBL);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("resources/shaders/texture.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("resources/textures/ml.png");
        m_Texture->Bind();
        m_Shader->SetUniform1i("u_Texture", 0);
        glm::vec3 m_TranslationA(200, 200, 0);
        glm::vec3 m_TranslationB(400, 200, 0);
	}

	void TestTexture2D::OnRender()
	{
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer renderer;

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = (camera ? camera->getViewProjection() : m_Proj * m_View) * model;

            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = (camera ? camera->getViewProjection() : m_Proj * m_View) * model;

            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}