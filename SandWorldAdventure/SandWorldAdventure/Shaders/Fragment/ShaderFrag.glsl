#version 330
// Global variable time
uniform float Time;

in vec3 uv;
out vec4 fragment;
void main()
{
    //vec2 u = uv.xy * 20;
    //fragment = vec4(floor(u.x)/20.0f,floor(u.y)/20.0f,0,1);
    float greyscale = (uv.x + uv.y) / 2.0;
    fragment = vec4(greyscale,greyscale,greyscale,1 );
}