#version 330

// Global variable time
uniform float Time;

in vec2 vPos;
in vec3 vUVCoord;

out vec3 uv; 
out vec3 color;
void main()
{
    gl_Position = vec4(vPos, 0, 1); //vec4(vPos + vec2(sin(vPos.x*6.282 + Time), cos(vPos.y*6.282 + Time))*0.2, 0.0, 1.0);
    uv = vUVCoord;
}