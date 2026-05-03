#version 330 core

layout(location=0) out vec4 fragmentColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform float u_materialData[8];

uniform sampler2D texture_diffuse;
uniform bool u_useTexture;

uniform vec3 lightPosWorld;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec3 baseColor = vec3(
        u_materialData[0],
        u_materialData[1],
        u_materialData[2]
    );

    vec3 specularComponent = vec3(
        u_materialData[3],
        u_materialData[4],
        u_materialData[5]
    );

    float shininess = u_materialData[6];
    float alpha = u_materialData[7];

    if(u_useTexture)
    {
        baseColor *= texture(texture_diffuse, TexCoords).rgb;
    }

    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPosWorld - FragPos);
    vec3 V = normalize(viewPos - FragPos);

    vec3 ambient = 0.1 * baseColor * lightColor;

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * baseColor * lightColor;

    vec3 H = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), shininess);
    vec3 specular = spec * specularComponent * lightColor;

    fragmentColor = vec4(ambient + diffuse + specular, alpha);
}