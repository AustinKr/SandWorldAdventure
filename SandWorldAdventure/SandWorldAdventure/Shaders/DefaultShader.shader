#VERTEX
    #version 330
    in vec2 vPos;
    in vec3 vUvCoord;

    out vec3 uv; 
    void main()
    {
        gl_Position = vec4(vPos, 0, 1);
        uv = vUvCoord;
    }
#ENDVERTEX
#FRAGMENT
    #version 330

    uniform vec4 ShadeColor;

    in vec3 uv;
    out vec4 fragment;
    void main()
    {
        fragment = vec4(ShadeColor);
    }
#ENDFRAGMENT