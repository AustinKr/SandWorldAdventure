#version 400

struct StaticQuadtreeNode
{
	// Index relative to this node's parents
	int Index;
	bool IsLeaf;
	unsigned int p_Data; // Could be 0( no child or leaf data), index array of 4 in buffer to child node, or leaf data( a tile color to be interpreted as an unsigned integer)
};
struct StaticQuadtree
{
	StaticQuadtreeNode* p_NodesBegin;
	int NodeCount;
};

// Uniform variables
uniform float Time;
uniform dvec2 TilemapOrigin;
uniform ivec2 TilemapBounds;
uniform dvec2 TilemapWorldSize;

//layout(std430, binding = 3) buffer tilemapQuadtreeLayout
//{
//    StaticQuadtree tree[];
//}
layout(std430, binding = 3) buffer layoutName
{
    int data_of_ints[];
}

in vec3 uv;
out vec4 fragment;
void main()
{
    int tileX = int(floor((double(uv.x) - TilemapOrigin.x) * double(TilemapBounds.x) / TilemapWorldSize.x)) + data_of_ints[0];
    int tileY = int(floor((double(uv.x) - TilemapOrigin.x) * double(TilemapBounds.x) / TilemapWorldSize.x));

    float greyscale = (float(tileX) + float(tileY)) / 2.0 / float(max(TilemapBounds.y, TilemapBounds.x));
    fragment = vec4(greyscale,greyscale,greyscale,1 );
}