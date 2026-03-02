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
    
    uniform sampler2D Tex;
    uniform vec4 Tex_RedMask;
    uniform vec4 Tex_GreenMask;
    uniform vec4 Tex_BlueMask;
    uniform vec4 Tex_AlphaMask;

    uniform vec4 ShadeColor;

    in vec3 uv;
    out vec4 fragment;
    void main()
    {
        // TODO: Use -height to determine whether to flip texture based on differing orgin points
        
        vec4 color = texture(Tex, uv.xy);
        fragment = (
            color.r * Tex_RedMask
            + color.g * Tex_GreenMask 
            + color.b * Tex_BlueMask 
            + color.a * Tex_AlphaMask)/ 256.0f * vec4(ShadeColor);
    }
#ENDFRAGMENT