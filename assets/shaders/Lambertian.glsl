#version 330 core
layout(location=0) out vec4 fragmentColor;
in vec3 FragPos;
in vec3 Normal;
uniform vec3 lightPosWorld;  
uniform vec3 lightColor;
uniform float u_materialData[8]; 
void main()
{
    vec3 baseColor = vec3(u_materialData[0], u_materialData[1], u_materialData[2]);
    float alpha = u_materialData[7];

    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPosWorld - FragPos);

    vec3 ambient = 0.08 * baseColor * lightColor;

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * baseColor * lightColor;

    fragmentColor = vec4(ambient + diffuse, alpha);
}