#shader vertex
#version 410 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
};


#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

void main()
{
    float red = 0.0;
    float green = 0.5;
    float blue = 1.0;
    color = vec4(red, green, blue, 1.0);   
};