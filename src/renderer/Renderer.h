#pragma once

#include <GL/glew.h>


#define ASSERT(x) \
    if (!(x))     \
        __debugbreak(); // __ is for the compiler, this only works for msvc

//! We should wrap every openGL function with this macro! (does need a cople more things to work everywhere like in ifs and assignments)
#define GLCall(x)   \
    GLClearError(); \
    x;              \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Clear all the error flags from openGL
void GLClearError();
// Callback to log errors
bool GLLogCall(const char *function, const char *file, int line);


