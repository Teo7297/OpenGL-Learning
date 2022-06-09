#pragma once

#include <GL/glew.h>
#include <VertexArray.h>
#include <IndexBuffer.h>
#include <Shader.h>


#define ASSERT(x) \
    if (!(x))     \
        __debugbreak(); // __ is for the compiler, this only works for msvc

#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Clear all the error flags from openGL
void GLClearError();
// Callback to log errors
bool GLLogCall(const char *function, const char *file, int line);

class Renderer
{
private:
    
public:
    // Renderer();
    // ~Renderer();
    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};
