#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoords;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

uniform float u_time; 
uniform vec3 u_blackHolePos;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;


void main(void)
{
    vec4 worldPos4 = modelMatrix * vec4(in_Position, 1.0);
    vec3 worldPos = worldPos4.xyz;

    vec3 toBH = worldPos - u_blackHolePos;
    float dist = length(toBH);
    vec3 dir = normalize(toBH);
    
    float strength = 2 / (dist + 2);


    float angle = strength * 20;
    
    float s = sin(angle);
    float c = cos(angle);

    vec3 rotatedPos = toBH;
    rotatedPos.x = toBH.x * c - toBH.z * s;
    rotatedPos.z = toBH.x * s + toBH.z * c;


    float stretch = 5 + (strength * 2.0);
    vec3 finalPos = u_blackHolePos + (rotatedPos / stretch);


    FragPos = finalPos;
    Normal = normalize(normalMatrix * in_Normal);
    TexCoords = in_TexCoords;

    gl_Position = projMatrix * viewMatrix * vec4(finalPos, 1.0);
}