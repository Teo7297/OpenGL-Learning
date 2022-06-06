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

//* Returning multiple objects from a function (using a tuple for example) is bad, instead it's better to use a struct like this:
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// here we return the struct with the parsed shaders! very cool
static ShaderProgramSource ParseShader(const std::string &filepath)
{
    // cpp style file handling is slower but easier to read than c-style
    std::ifstream stream(filepath); //? open the file

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::stringstream ss[2]; //? stack allocated array of stringstream, one for vertex one for fragment

    //* Parse line by line, getline is part of string lib
    std::string line;
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        //* if first line check the type of the shader we are parsing
        if (line.find("#shader") != std::string::npos) //? npos means it did NOT find it, so not not :)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        //* otherwise parse the next line of the shader file
        else
        {
            //? Using enum values to actually determine the shader position in the array (wow)
            ss[(int)type] << line << "\n";
        }
    }
    return ShaderProgramSource{ss[0].str(), ss[1].str()};
}

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

        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        // Vertex settings
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        // Index buffer openGL (ibo = Index Buffer Object)
        IndexBuffer ib(indices, 6);

        // Shaders
        ShaderProgramSource source = ParseShader("res\\shaders\\Basic.shader");
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader));

        // Uniforms
        int location = glGetUniformLocation(shader, "u_Color"); // Should check location != -1
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        // Unbind everything
        GLCall(glBindVertexArray(0));
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increment = 0.01f;
        // Render cycle
        while (!glfwWindowShouldClose(window))
        {
            glClear(GL_COLOR_BUFFER_BIT);

            GLCall(glUseProgram(shader));                       //* bind shader
            GLCall(glUniform4f(location, r, 0.0f, 0.0f, 1.0f)); //* set uniform

            GLCall(glBindVertexArray(vao));
            ib.Bind(); //* bind vertex buffer

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); //* drawCall

            glfwSwapBuffers(window);
            glfwPollEvents();

            if (r > 1.0f)
                increment = -0.01f;
            else if (r < 0.0f)
                increment = 0.01f;
            r += increment;
        }

        GLCall(glDeleteProgram(shader));
    }
    glfwTerminate();

    return 0;
}