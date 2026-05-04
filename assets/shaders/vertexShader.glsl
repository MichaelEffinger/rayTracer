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

    // 1. Enhanced Strength Falloff
    // Using a power of 2 or 3 makes the pull much more dramatic near the center
    float strength = 5 / (dist + 2);

    // 2. The "Looping" Math (Rotation)
    // We create an angle that gets massive as dist gets small
    // Multiplying by u_time makes it spin; using a large constant makes it static-wrap
    float angle = strength * 20; // Increase 10.0 to get more "loops"
    
    float s = sin(angle);
    float c = cos(angle);
    
    // Rotate the position around the 'up' axis (Y-axis swirl)
    // This is a standard 2D rotation matrix applied to the X and Z planes
    vec3 rotatedPos = toBH;
    rotatedPos.x = toBH.x * c - toBH.z * s;
    rotatedPos.z = toBH.x * s + toBH.z * c;

    // 3. Apply Spaghettification (The Stretch)
    // Pull the vertex toward the center as it spins
    float stretch = 5 + (strength * 2.0);
    vec3 finalPos = u_blackHolePos + (rotatedPos / stretch);

    // 4. Output
    FragPos = finalPos;
    Normal = normalize(normalMatrix * in_Normal);
    TexCoords = in_TexCoords;

    gl_Position = projMatrix * viewMatrix * vec4(finalPos, 1.0);
}