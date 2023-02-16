#version 330 core
out vec4 FragColor;

uniform vec3 linghtingColor;

void main()
{
    FragColor = vec4(linghtingColor, 1.0f);
}