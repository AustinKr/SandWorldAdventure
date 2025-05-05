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
		int Index; // Index relative to this node's parents
		bool IsNull; // If set to true, TileColor and ChildrenIndices will be not have values
		bool IsLeaf;

		vec3 TileColor; // Only is initialized if IsLeaf
		int ChildrenIndicies[4]; // Null if IsLeaf
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
    uniform dvec2 TilemapOrigin; // In screen coordinates
    uniform ivec2 TilemapBounds; 

    in vec3 uv;
    out vec4 fragment;
    
    void main()
    {
        int index = int(floor(uv.x * float(Nodes.length())));
        fragment = vec4(Nodes[index].TileColor, 1);
        return;


        int tileX = int(floor((double(uv.x) - TilemapOrigin.x) * double(TilemapBounds.x)));
        int tileY = int(floor((double(uv.y) - TilemapOrigin.y) * double(TilemapBounds.y)));

        //// Test tileXY
        //fragment = vec4(float(tileX) / float(TilemapBounds.x), float(tileY) / float(TilemapBounds.y), 0, 1);
        //return;


        // Start at the root node, iterate through each node containing the tile
        int i = 0;
        vec2 parentNodeOrigin = RootNodeOrigin; // Starts as the root node
        for(int d = 1; d < LeafDepth; d++)
        {
            if(Nodes[i].IsNull) // We should reach the end of the loop for a success- not a null node 
            {
                i = -1;
                break;
            }
            // Eq. for the size of a node given the depth(d): s = RootSize / pow(2, d);
            float currentNodeSize = RootNodeSize / float(pow(2, d));
            // Convert tile position to node position (relative to its parent)
            int nodeC = int(floor(float(tileX - parentNodeOrigin.x) / currentNodeSize));
            int nodeY = int(floor(float(tileY - parentNodeOrigin.y) / currentNodeSize));
            
            // Use the column and row to add to the index
            i += nodeC + nodeY * 2;
            // Update the parent position
            parentNodeOrigin = vec2(parentNodeOrigin.x + nodeC * currentNodeSize, parentNodeOrigin.y + nodeY * currentNodeSize); // Set it to the parent origin plus the distance of relative nodes, in the base unit(tiles)
            if(parentNodeOrigin.x < RootNodeOrigin.x || parentNodeOrigin.y < RootNodeOrigin.y ||
                parentNodeOrigin.x + currentNodeSize > RootNodeSize || parentNodeOrigin.y + currentNodeSize > RootNodeSize)
            {
                i = -1;
                break;
            }
        }
        if(i == -1)
            fragment = vec4(1,0,1,1); // null color
        else if (i >= Nodes.length())
            fragment = vec4(.5,0,.5,1); // null color
        else
            fragment = vec4(Nodes[i].TileColor, 1);
            
        //float greyscale = (float(tileX) + float(tileY)) / 2.0 / float(max(TilemapBounds.y, TilemapBounds.x));
        //fragment = vec4(greyscale,greyscale,greyscale,1 );
        
    }
#ENDFRAGMENT