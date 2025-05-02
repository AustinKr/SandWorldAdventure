#VERTEX
    #version 410
    
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
    #version 430
    
    struct QuadtreeNode
    {
        int Index;// Index relative to this node's parents
    	bool IsLeaf;
        //TODO: Separate data
    	unsigned int p_Data; // Could be 0( no child or leaf data), index array of 4 in buffer to child node, or leaf data( a tile color to be interpreted as an unsigned integer)
    };

    layout(std430, binding = 0) buffer QuadtreeBlock
    {
        //int NodeCount;
        vec2 RootNodeOrigin; // TODO: This should combine with tilemaporigin when the quadtree is specialized for bitmaps; 
        float RootNodeSize; // TODO: This should combine with tilemapbounds when the quadtree is specialized for bitmaps
        int LeafDepth;
        
    	QuadtreeNode Nodes[];
    }; // No instance identifier, so these are global variables

    // Uniform variables
    uniform dvec2 TilemapOrigin;
    uniform ivec2 TilemapBounds;
    uniform dvec2 TilemapWorldSize;

    in vec3 uv;
    out vec4 fragment;
    
    void main()
    {
        /*unsigned int i = int(floor(uv.x * 100.0));
        if(Nodes.length() > 0 && i < Nodes.length() && i >= 0)
            fragment = vec4(0, Nodes[i].p_Data, 0, 1);
        else
            fragment = vec4(1,0,0,1);

        return;*/
        
        int tileX = int(floor((double(uv.x) - TilemapOrigin.x) * double(TilemapBounds.x) / TilemapWorldSize.x));
        int tileY = int(floor((double(uv.y) - TilemapOrigin.y) * double(TilemapBounds.y) / TilemapWorldSize.y));

        //// Test tileXY
        //fragment = vec4(float(tileX) / float(TilemapBounds.x), float(tileY) / float(TilemapBounds.y), 0, 1);
        //return;

        // Start at the root node, iterate through each node containing the tile
        int i = 0;
        vec2 currentNodeOrigin = RootNodeOrigin; // Starts as the root node
        for(int d = 1; d < LeafDepth; d++)
        {
            if(Nodes[i].p_Data == 0)
            {
                i = -1;
                break;
            }
            // Eq. for the size of a node given the depth(d): s = RootSize / pow(2, d);
            double currentNodeSize = RootNodeSize / float(pow(2, d));
            // Convert tile position to node position (relative to its parent)
            int nodeC = int(floor(double(tileX - currentNodeOrigin.x) / currentNodeSize));
            int nodeY = int(floor(double(tileY - currentNodeOrigin.y) / currentNodeSize));

            // Use the column and row to add to the index
            i += nodeC + nodeY * 2;
            // Update the parent position
            currentNodeOrigin = vec2(currentNodeOrigin.x + nodeC * currentNodeSize, currentNodeOrigin.y + nodeY * currentNodeSize); // Set it to the parent origin plus the distance of relative nodes, in the base unit(tiles)
        }
        if(i == -1)
            fragment = vec4(1,0,1,1); // null color
        else if (i >= Nodes.length())
            fragment = vec4(.5,0,.5,1); // null color
        else
            fragment = vec4(Nodes[i].p_Data, 0, 0, 1);//TODO: Parse color if possible
            
        //float greyscale = (float(tileX) + float(tileY)) / 2.0 / float(max(TilemapBounds.y, TilemapBounds.x));
        //fragment = vec4(greyscale,greyscale,greyscale,1 );
        
    }
#ENDFRAGMENT