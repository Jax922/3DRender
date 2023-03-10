#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal;

out vec2 ourTexCoord;
out vec3 normal;
out vec3 fragPos;
//out float ourAlpha;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform float alpha;


void main()
{   
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourTexCoord = aTex;
    normal = aNormal;
    fragPos = vec3(model * vec4(aPos,1.0f));
    ///ourAlpha = alpha;
}