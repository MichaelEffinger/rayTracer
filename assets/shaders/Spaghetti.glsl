#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TexCoords;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

uniform vec3 u_blackHolePos;
uniform float u_twistFactor;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main(void)
{
    // world space position
    vec4 worldPos4 = modelMatrix * vec4(in_Position, 1.0);
    vec3 worldPos = vec3(worldPos4);

    // vector toward black hole
    vec3 toBH = worldPos - u_blackHolePos;
    float dist = length(toBH);
    vec3 dir = normalize(toBH);

    // falloff (strong near center, weak far away)
    float strength = 10.0 / (1.0 + dist * dist);

    // radial pull inward
    vec3 radial = -dir * strength * 2.0;

    // stable perpendicular direction for swirling
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 tangent = normalize(cross(up, dir));

    // twist around black hole
    vec3 swirl = tangent * strength * u_twistFactor;

    // apply deformation
    worldPos += radial;
    worldPos += swirl;

    FragPos = worldPos;
    Normal = normalize(normalMatrix * in_Normal);
    TexCoords = in_TexCoords;

    gl_Position = projMatrix * viewMatrix * vec4(worldPos, 1.0);
}