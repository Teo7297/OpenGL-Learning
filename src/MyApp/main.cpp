//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#pragma warning(disable : 4244)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <logger.h>

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        Logger::error("Glew init failed!");
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << "\n";

    //* data for the buffer to render
    float positions[6] =
    {
        -0.5f, -0.5f,   // position of vertex 1
         0.0f,  0.5f,   // position of vertex 2
         0.5f, -0.5f    // position of vertex 3
    };

    //* id for the buffer that we are generating
    unsigned int buffer;
    //* generate 1 buffer
    glGenBuffers(1, &buffer);

    //* select the generated buffer as a normal array buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    //* set the size(in bytes) or directly initialize it with vertex data
    // choose between static and dynamic to tell if data will be modified or not and draw or read depending on what we do with the data (we draw static now)
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // Enable the vertex
    glEnableVertexAttribArray(0);
    // Set vertex attribute POSITION
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //* drawCall
        glDrawArrays(GL_TRIANGLES, 0, 3);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}