#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 colour;
uniform float sinVal = 1.0;

void main()
{
  colour = aColor*sinVal;
  gl_Position = vec4(aPos.x + (0.4 * sinVal), aPos.y, aPos.z, 1.0);
};