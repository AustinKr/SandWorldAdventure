#VERTEX
    #version 330
    //// Global variable time
    //uniform float Time;

    in vec2 vPos;
    in vec3 vUVCoord;

    out vec3 uv; 
    out vec3 color;
    void main()
    {
        gl_Position = vec4(vPos, 0, 1); //vec4(vPos + vec2(sin(vPos.x*6.282 + Time), cos(vPos.y*6.282 + Time))*0.2, 0.0, 1.0);
        uv = vUVCoord;
    }
#ENDVERTEX
#FRAGMENT
    #version 330
    //// Global variable time
    //uniform float Time;

    in vec3 uv;
    out vec4 fragment;
    void main()
    {
        //vec2 u = uv.xy * 20;
        //fragment = vec4(floor(u.x)/20.0f,floor(u.y)/20.0f,0,1);
        float greyscale = (uv.x + uv.y) / 2.0f;
        fragment = vec4(greyscale,greyscale,greyscale,1 );
    }
#ENDFRAGMENT