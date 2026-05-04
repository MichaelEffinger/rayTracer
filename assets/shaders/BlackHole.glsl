#version 330 core
layout(location=0) out vec4 fragmentColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPosWorld;  
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float u_materialData[8]; 

void main()
{
    vec3 baseColor = vec3(u_materialData[0], u_materialData[1], u_materialData[2]);
    float alpha = u_materialData[7];

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    float rim = 1.0 - max(dot(N, V), 0.0);

    rim = pow(rim, 2);

    vec3 black = vec3(0.0);
    vec3 color = mix(black, baseColor, rim);

    fragmentColor = vec4(color, alpha);
}