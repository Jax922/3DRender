#version 330 core
out vec4 FragColor;

in vec2 ourTexCoord;
in float ourAlpha;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D texture1;

uniform bool isTexture;
uniform bool isColor;

uniform float alpha;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float shininess;

void main()
{

    // Ambient
	float ambientStrength = 0.2f;
	vec3 ambient = lightColor * ambientStrength;

	// Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos-fragPos);
	float diff = max(dot(norm,lightDir),0.0f);
	vec3 diffuse = lightColor * diff;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0f), shininess);
	vec3 specular = lightColor * spec * specularStrength;

	vec3 result = (ambient + diffuse + specular) * objectColor;

    // FragColor = vec4(ourColor, ourAlpha);
    FragColor = texture(texture1, ourTexCoord);
    if(isTexture && isColor)
    {
        FragColor = texture(texture1, ourTexCoord) * vec4(result, 1.0f);
    }
    else if (isTexture && !isColor)
    {
        FragColor = texture(texture1, ourTexCoord);
    }
    else if (!isTexture && isColor)
    {
        FragColor = vec4(result, 1.0f);
    }
    else 
    {
        FragColor = vec4(1.0f);
    }
} 