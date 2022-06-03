//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")  // hide the command prompt when launching application, disable output prints
#pragma warning(disable : 4244)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <logger.h>

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();

    //* Replaces the current source code in the shader object (initially it's the default shader code!)
    // It needs: id of the shader, number of sources (number of elements in the next 2 params), the array of pointers to the strings of the source codes, array of string lengths (if we pass nullptr here, it considers every string to be null terminated (c-style string, thats why we used the c_str() function above!) )
    glShaderSource(id, 1, &src, nullptr);

    //* Start shader compilation
    glCompileShader(id);

    //* handle eventual errors here
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) // error
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // char message[length]; we cannot do this in c++... so we need a hacky way..
        // we still want this to be allocated on the stack so we use the alloca function (malloc is for heap allocation) (yay)
        //# The alloca() function returns a pointer to the beginning of the allocated space. If the allocation causes stack overflow, program behaviour is undefined.
        char *message = (char *)alloca(length * sizeof(char));

        glGetShaderInfoLog(id, length, &length, message);
        std::cout
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n"
            << message
            << std::endl;

        glDeleteShader(id);

        return 0;
    }

    return id;
}

//# Usually we would take a shader from an external file, for now we will use a simple string
/**
 * @brief Create a Shader object starting from a vertex and a fragment shader (string!)
 *
 * @param vertexShader vertex shader code string pointer
 * @param fragmentShader fragment shader code string pointer
 * @return int indentifier of the created shader object
 */
static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    //* This is basically the same as glGenBuffers in the main (just inconsistently implemented, returns the index instead of taking the pointer..)
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //? Now, we have the compiled shaders' files, we need to attach those to our rendering pipeline
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);     // program linking to our executable, state of linking is stored in the program object
    glValidateProgram(program); // program validation, state of validation is stored in the program object

    //? When using msvc, shaders compilation results in .obj intermediate files, so we can actually delete the shader objects and free some memory
    //? Note that this deletes the source code and makes it impossible to debug.
    //? Note that this only sets a delete flag, to be actually deleted we need to detach with glDetachShader()!!!!
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
            -0.5f, -0.5f, // position of vertex 1
            0.0f, 0.5f,   // position of vertex 2
            0.5f, -0.5f   // position of vertex 3
        };

    //* id for the buffer that we are generating
    unsigned int buffer;
    //* generate 1 buffer
    glGenBuffers(1, &buffer);

    //* select the generated buffer as a normal array buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    //* set the size(in bytes) or directly initialize it with vertex data
    // choose between static and dynamic to tell if data will be modified or not, and draw or read depending on what we do with the data (we draw static now)
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // Enable the vertex
    glEnableVertexAttribArray(0);
    // Set vertex attribute POSITION
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    std::string vertexShader =
        // 330 is the version, core uses non deprecated functions
        "#version 410 core\n"
        "\n"
        // location is the index of our vertex attrib
        // gl_Position wants a vec4, we could convert it manually later but it's better to do this here.
        // it knows we gave a vec2, we passed 2 as argument in vertexAttribPointer
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 410 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        // 4th parameter is the alpha value, colors are floats between 0.0 and 1.0 (for no HDR at least)
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    //! We still need to bind the shader
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        //* drawCall
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();

    return 0;
}