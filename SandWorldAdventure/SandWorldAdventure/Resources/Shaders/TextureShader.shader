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
    uniform vec4 Tex_Resolution;
    uniform vec4 Tex_RedMask;
    uniform vec4 Tex_GreenMask;
    uniform vec4 Tex_BlueMask;
    uniform vec4 Tex_AlphaMask;

    uniform vec4 Tex_Color;

    uniform vec2 Outline_Thickness;
    uniform vec4 Outline_Color;
    uniform float Outline_Clip;

    in vec3 uv;
    out vec4 fragment;

    vec4 SampleTexture(vec2 uv)
    {
        // TODO: Use -height to determine whether to flip texture based on differing orgin points
        vec4 color = texture(Tex, uv);
        return 
            (color.r * Tex_RedMask
            + color.g * Tex_GreenMask 
            + color.b * Tex_BlueMask 
            + color.a * Tex_AlphaMask)/ 256.0f;
    }

    void main()
    {
        vec2 coord = uv.xy;
        fragment = SampleTexture(coord) * vec4(Tex_Color);

        // Try draw outline
        if(Outline_Thickness.x < 1 && Outline_Thickness.y < 1)
            return;
        vec2 thickness = Outline_Thickness / Tex_Resolution.xy;

        vec2 right = coord + vec2(thickness.x, 0),
             left = coord - vec2(thickness.x, 0),
             top = coord + vec2(0, thickness.y),
             bottom = coord - vec2(0, thickness.y);

        if(right.x > 1.0f || left.x < 0.0f || top.y > 1.0f || bottom.y < 0.0f ||
            SampleTexture(right).a < Outline_Clip || SampleTexture(left).a < Outline_Clip || SampleTexture(top).a < Outline_Clip || SampleTexture(bottom).a < Outline_Clip)
            fragment = Outline_Color;
    }
#ENDFRAGMENT