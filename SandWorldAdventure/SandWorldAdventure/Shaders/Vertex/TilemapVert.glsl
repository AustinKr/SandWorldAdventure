#version 400

// Uniform variables
uniform float Time;
uniform dvec2 TilemapOrigin;
uniform ivec2 TilemapBounds;
uniform dvec2 TilemapWorldSize;

in vec2 vPos;
in vec3 vUVCoord;

out vec3 uv;

void main()
{
    gl_Position = vec4(vPos, 0, 1);
    uv = vUVCoord;
}