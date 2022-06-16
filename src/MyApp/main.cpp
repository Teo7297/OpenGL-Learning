//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")  // hide the command prompt when launching application, disable output prints
#pragma warning(disable : 4244)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <Renderer.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


int main(void)
{
    // GLFW boilerplate code
    GLFWwindow *window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // openGL v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use openGL core profile

    window = glfwCreateWindow(640, 480, "OpenGL Learning Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //# Vertical sync / limit fps

    // GLEW boilerplate code
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << "\n";

    { // This scope is to fix the issue of the vertexBuffer being destroyed AFTER the context because its stack allocated

        // Vertex buffer
        float positions[] =
            {
                // vertices with texture coords
                -0.5f, -0.5f, 0.0f, 0.0f, // vertex 0 
                0.5f, -0.5f, 1.0f, 0.0f,  // vertex 1
                0.5f, 0.5f, 1.0f, 1.0f,   // vertex 2
                -0.5f, 0.5f, 0.0f, 1.0f   // vertex 3
            };

        // Index buffer
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0};

        GLCall(glEnable(GL_BLEND));                                 //# Texture blending enable
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC1_ALPHA)); //# Texture blending function setting

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Index buffer openGL (ibo = Index Buffer Object)
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);  // We create a 4x4 orthographic matrix
                                                                             // We specify values to adhere to the 4/3 aspect ratio
                                                                             // An ortho matrix is a way to map coords into a 2D plane where "far" objects do not get smaller.
                                                                             // It's the opposite of a perspective matrix
                                                                             //? This is perfect for 2D stuff







        // Shaders
        Shader shader("res\\shaders\\Basic.shader");
        shader.Bind();

        // Uniforms
        // shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Textures
        Texture texture("res\\textures\\avatar.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        shader.SetUniformMat4f("u_MVP", proj);

        // Unbind everything
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // Render cycle
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            shader.Bind();

            renderer.Draw(va, ib, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}