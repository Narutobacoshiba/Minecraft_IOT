#ifndef BLOCK_HPP
#define BLOCK_HPP
#include <glm/glm.hpp>
#include <vector>

#define BLOCKTYPE_GRASS 1
#define BLOCKTYPE_SAND 2
#define BLOCKTYPE_WATER 3
#define BLOCKTYPE_STONE 4
#define BLOCKTYPE_WOOD 5
#define BLOCKTYPE_COAL 6
#define BLOCKTYPE_GLASS 7
#define BLOCKTYPE_DIRT 8
#define BLOCKTYPE_LEAVE 9

struct Vertex {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

struct DataBuffer {
	void *data;
	size_t index, count, capacity;
};


const float cube_vertices[72] = {
   -1, 1, 1,  -1, 1,-1,   1, 1,-1,   1, 1, 1, // top (1,2,3,4)
   -1,-1, 1,  -1,-1,-1,   1,-1,-1,   1,-1, 1, // bottom (5,6,7,8)
	1,-1, 1,   1, 1, 1,   1, 1,-1,   1,-1,-1, // right (8,4,3,7)
   -1,-1,-1,  -1, 1,-1,  -1, 1, 1,  -1,-1, 1, // left (6,2,1,5)
   -1,-1, 1,  -1, 1, 1,   1, 1, 1,   1,-1, 1, // front (5,1,4,8)
    1,-1,-1,   1, 1,-1,  -1, 1,-1,  -1,-1,-1  // back (7,3,2,6)

    /*
   -1, 1, 1, // 1
   -1, 1,-1, // 2
    1, 1,-1, // 3
	1, 1, 1, // 4
   -1,-1, 1, // 5
   -1,-1,-1, // 6
    1,-1,-1, // 7
	1,-1, 1  // 8
	*/
};

/*
const float cube_coords[4][6] = { // (TOP, BOTTOM, SIDE)
	{1, 0, 0, 1, 0, 0}, // GRASS
	{1, 1, 1, 1, 1, 1}, // SAND
	{2, 0, 2, 0, 2, 0}, // BRICK
	{2, 1, 2, 1, 2, 1} // STONE
};*/

const float cube_coords[9][6] = { // (TOP, BOTTOM, SIDE)
	{0, 15, 2, 15, 1, 15}, // GRASS
	{0, 14, 0, 14, 0, 14}, // SAND
	{0, 13, 0, 13, 0, 13}, // WATER
	{3, 15, 3, 15, 3, 15}, // STONE
	{3, 14, 3, 14, 2, 14}, // WOOD
	{4, 15, 4, 15, 4, 15}, // COAL
	{1, 14, 1, 14, 1, 14}, // GLASS
	{2, 15, 2, 15, 2, 15}, // DIRT
	{4, 14, 4, 14, 4, 14}, // LEAVE
};

void add_cube_buffer(DataBuffer *vertex_buffer, DataBuffer *indice_buffer, glm::vec3 position, int data, float n);

#endif