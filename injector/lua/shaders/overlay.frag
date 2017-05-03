#version 330 core

in vec2 f_texcoord;
uniform sampler2D tex;

out vec4 outColor;

void main()
{
    outColor = texture(tex, f_texcoord);
}
