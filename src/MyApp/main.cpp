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
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>

int main(void)
{
    // GLFW boilerplate code
    GLFWwindow *window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // openGL v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use openGL core profile

    window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
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

    // Vertices buffer
    { // This scope is to fix the issue of the vertexBuffer being destroyed AFTER the context because its stack allocated
        float positions[] =
            {
                -0.5f, -0.5f, // vertex 0
                0.5f, -0.5f,  // vertex 1
                0.5f, 0.5f,   // vertex 2
                -0.5f, 0.5f,  // vertex 3
            };

        // Index buffer
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0};

        // Vertex buffer openGL
        unsigned int vao;
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Index buffer openGL (ibo = Index Buffer Object)
        IndexBuffer ib(indices, 6);

        // Shaders
        Shader shader("res\\shaders\\Basic.shader");
        shader.Bind();

        // Uniforms
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Unbind everything
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        float r = 0.0f;
        float increment = 0.01f;
        // Render cycle
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.0f, 0.0f, 1.0f);

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //* drawCall

            glfwSwapBuffers(window);
            glfwPollEvents();

            if (r > 1.0f)
                increment = -0.01f;
            else if (r < 0.0f)
                increment = 0.01f;
            r += increment;
        }
    }
    glfwTerminate();

    return 0;
}