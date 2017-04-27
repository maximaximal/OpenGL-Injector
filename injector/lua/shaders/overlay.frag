#version 330 core

in vec2 f_texcoord;
uniform sampler2D tex;

void main()
{
    //gl_FragColor = texture(tex, texcoord);
    gl_FragColor = texture(tex, f_texcoord);
    //gl_FragColor = texture(tex, texcoord);
}
