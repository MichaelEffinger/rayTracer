#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform vec3 lightPosWorld;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform float u_materialData[8];


out vec4 fragColor;

const float PI = 3.14159265359;


void main(){

    vec3 C_lambda= vec3(
        u_materialData[0],
        u_materialData[1],
        u_materialData[2]
    );

    float r = max(u_materialData[3], 0.001);
    float p = u_materialData[4];

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPosWorld - FragPos);
    vec3 H = normalize(V+L);

    float NdotL = max(dot(N,L), 0.001);
    float NdotV = max(dot(N,V), 0.001);
    float NdotH = max(dot(N,H), 0.001);
    float VdotH = max(dot(V,H), 0.001);

    // fresnal factor approximation
    vec3 Fresnal = C_lambda + (1.0 - C_lambda) * pow(1.0 - VdotH, 5.0);


    // geometric term
    float Gv = NdotV / (r - r * NdotV + NdotV);
    float Gl = NdotL / (r - r * NdotL + NdotL);
    float Geometric = Gv * Gl;

    // distrbution term
    float t2 = pow(NdotH,2);
    float denom =  1.0 + r * t2 - t2;
    float Distrobution = r / pow(denom,2);

    vec3 specular = (Fresnal * Geometric * Distrobution) / max(4.0 * NdotV * NdotL, 0.001);

    vec3 diffuse = C_lambda * (1.0 - Geometric) * (1.0 / PI);

    vec3 color = (specular + diffuse) * lightColor * NdotL;
    // vec3 color = brdf * lightColor * NdotL;

    //fragColor = vec4(N * 0.5 + 0.5, 1.0);
    fragColor = vec4(color, 1.0);

}



