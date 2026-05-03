#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoords;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix; 

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords; // Pass this to Fragment Shader

void main(void)
{
    vec4 worldPos = modelMatrix * vec4(in_Position, 1.0);
    FragPos = vec3(worldPos);
    
    Normal = normalize(normalMatrix * in_Normal);
    
    TexCoords = in_TexCoords; 
    
    gl_Position = projMatrix * viewMatrix * worldPos;
}