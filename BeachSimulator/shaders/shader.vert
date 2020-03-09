#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 color;
uniform float t;
out vec3 norm;
out vec3 FragPos;
vec3 pos2;

float frequency = 4.0;
float amplitude = 0.2;
float timestep = 100.0;
vec3 dv_dx;
vec3 dv_dz;
vec3 n;
void main()
{
    
    pos2 = position;
    pos2.y = amplitude * (sin(frequency * position.x + t/timestep ) - cos(frequency * position.z + t/timestep));
    
    dv_dx = vec3(1.0, amplitude*cos(frequency * position.x+t/timestep) * frequency, 0.0);
    dv_dz = vec3(0.0, amplitude*cos(frequency * position.z+t/timestep) * frequency, 1.0);
    n = cross(dv_dx, dv_dz);
    norm = normalize(n);

    gl_Position = projection * view * model * vec4(pos2, 1.0);

    FragPos = vec3(model * vec4(position, 1.0));
    
    
    //normalizedNormal = (0.5f * normalize(n)) + 0.5f;
    //normalizedNormal = color;
}
