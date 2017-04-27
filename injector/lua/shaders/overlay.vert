#version 330 core

in vec2 position;
in vec2 texcoord;

out vec2 f_texcoord;

void main()
{
    f_texcoord = texcoord;

    gl_Position = position;
}
