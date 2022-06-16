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

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

int main(void)
{
    // GLFW boilerplate code
    GLFWwindow *window;
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // openGL v3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use openGL core profile

    window = glfwCreateWindow(960, 540, "OpenGL Learning Project", NULL, NULL);
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
                100.0f, 100.0f, 0.0f, 0.0f, // vertex 0
                200.0f, 100.0f, 1.0f, 0.0f, // vertex 1
                200.0f, 200.0f, 1.0f, 1.0f, // vertex 2
                100.0f, 200.0f, 0.0f, 1.0f  // vertex 3
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

        // MVP matrix
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
        glm::mat4 proj = glm::ortho(-0.0f, 960.0f, -0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 mvp = proj * view * model; //? Mind the reverse order!

        // Shaders
        Shader shader("res\\shaders\\Basic.shader");
        shader.Bind();

        // Uniforms
        // shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Textures
        Texture texture("res\\textures\\avatar.png");
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);

        shader.SetUniformMat4f("u_MVP", mvp);

        // Unbind everything
        va.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        // ImGUI setup
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO(); 
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        // Render cycle
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            shader.Bind();

            renderer.Draw(va, ib, shader);

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
                ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}