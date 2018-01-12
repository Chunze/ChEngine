#version 330 core
out vec4 FragColor;

in vec3 vertexColor;

uniform float greenValue;


void main()
{
    FragColor = vec4(0.0, greenValue, 0.0, 1.0);
} 