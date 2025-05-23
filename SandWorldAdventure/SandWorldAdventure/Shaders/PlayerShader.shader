#VERTEX
    #version 330
    in vec2 vPos;
    in vec3 vUvCoord;

    out vec3 uv; 
    out vec3 color;
    void main()
    {
        gl_Position = vec4(vPos, 0, 1);
        uv = vUvCoord;
    }
#ENDVERTEX
#FRAGMENT
    #version 330

    in vec3 uv;
    out vec4 fragment;
    void main()
    {
        float greyscale = (uv.x + uv.y) / 2.0f;
        fragment = vec4(greyscale,greyscale/2.0f,greyscale/2.0f,1 );
    }
#ENDFRAGMENT