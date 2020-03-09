#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

uniform vec3 color;

in vec3 norm;
in vec3 FragPos;
out vec4 fragColor;

vec3 lightDirection = vec3(0.5, 0.5, 0.5);
float ambientStrength = 0.8;
float diffuseStrength = 0.5;
float specularStrength = 0.3;

vec3 ambient_color;
vec3 diffuse_color;
vec3 specular_color;
vec3 result;
float spec;
vec3 reflectDir;
vec3 viewDir;
float diff;
vec3 norm2 = vec3(0.0, 1.0, 0.0);
uniform samplerCube skybox;

void main()
{
    ambient_color = ambientStrength * color;
    diff = max(dot(norm, lightDirection), 0.0);
    diffuse_color = diffuseStrength * diff * color;

    viewDir = normalize(vec3(0.0f, 3.0f, 3.0f) - FragPos);
    reflectDir = reflect(-lightDirection, norm); 
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
    specular_color = specularStrength * spec * color;  


    result = ambient_color + diffuse_color + specular_color;

    //fragColor = vec4(result, 1.0f);
    vec3 I = normalize(vec3(0.0f, 5.0f, 0.0f) - FragPos);
    vec3 R = -reflect(I, normalize(norm2));
    fragColor = 0.3*vec4(texture(skybox, R).rgb, 1.0f) + vec4(result, 1.0f);
}
