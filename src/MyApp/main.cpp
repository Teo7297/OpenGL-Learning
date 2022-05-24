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

    float positions[6] =
    {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // id for the buffer that we are generating
    unsigned int buffer;
    // generate 1 buffer
    glGenBuffers(1, &buffer);

    // select the generated buffer as a normal array buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // set the size(in bytes) or directly initialize it with vertex data
    // choose between static and dynamic to tell if data will be modified or not and draw or read depending on what we do with the data (we draw static now)
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

        /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //# Glew OpenGL
        // drawCall
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //# Legacy OpenGL
        // glBegin(GL_TRIANGLES);
        // glVertex2f(-0.5f, -0.5f);
        // glVertex2f( 0.0f,  0.5f);
        // glVertex2f( 0.5f, -0.5f);
        // glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
}