#VERTEX
    #version 400

    in vec2 vPos;
    in vec3 vUVCoord;

    out vec3 uv;

    void main()
    {
        gl_Position = vec4(vPos, 0, 1);
        uv = vUVCoord;
    }
#ENDVERTEX
#FRAGMENT
    #version 400

    //struct StaticQuadtreeNode
    //{
    //	// Index relative to this node's parents
    //	int Index;
    //	bool IsLeaf;
    //	unsigned int p_Data; // Could be 0( no child or leaf data), index array of 4 in buffer to child node, or leaf data( a tile color to be interpreted as an unsigned integer)
    //};
    //struct StaticQuadtree
    //{
    //	StaticQuadtreeNode* p_NodesBegin; // TODO: This has to be unsigned long long I think
    //	int NodeCount;
    //};

    //layout(std430, binding = 3) buffer tilemapQuadtreeLayout
    //{
    //    StaticQuadtree tree[];
    //}
    //layout(binding = 3) in buffer layoutName
    //{
    //    int data_of_ints[];
    //}

    // Uniform variables
    //uniform float Time;
    uniform dvec2 TilemapOrigin;
    uniform ivec2 TilemapBounds;
    uniform dvec2 TilemapWorldSize;

    in vec3 uv;
    out vec4 fragment;
    void main()
    {
        int tileX = int(floor((double(uv.x) - TilemapOrigin.x) * double(TilemapBounds.x) / TilemapWorldSize.x));// + data_of_ints[0];
        int tileY = int(floor((double(uv.x) - TilemapOrigin.x) * double(TilemapBounds.x) / TilemapWorldSize.x));

        float greyscale = (float(tileX) + float(tileY)) / 2.0 / float(max(TilemapBounds.y, TilemapBounds.x));
        fragment = vec4(greyscale,greyscale,greyscale,1 );
    }
#ENDFRAGMENT