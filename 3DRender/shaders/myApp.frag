#version 330 core
out vec4 FragColor;

uniform sampler2D texture1;

in vec2 ourTexCoord;
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor, 1.0f);
    //FragColor = texture(texture1, ourTexCoord);
} 