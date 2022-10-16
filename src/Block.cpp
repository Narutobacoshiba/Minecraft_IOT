#include "Block.hpp"
#include <iostream>

void add_cube_buffer(DataBuffer *vertex_buffer, DataBuffer *indice_buffer, glm::vec3 position, int data, float n) {
	int current_vb_indx = vertex_buffer->index;
	float m = 1. / 16;
	int type = data - 1;

	float* vertex_data = (float*)vertex_buffer->data;
	int* indice_data = (int*)indice_buffer->data;

	// vertices | normal | texture coordinates
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			
			vertex_data[vertex_buffer->count++] = position.x + n * cube_vertices[i * 12 + j * 3];
			vertex_data[vertex_buffer->count++] = position.y + n * cube_vertices[i * 12 + j * 3 + 1];
			vertex_data[vertex_buffer->count++] = position.z + n * cube_vertices[i * 12 + j * 3 + 2];

			vertex_data[vertex_buffer->count++] = cube_vertices[i * 12 + j * 3];
			vertex_data[vertex_buffer->count++] = cube_vertices[i * 12 + j * 3 + 1];
			vertex_data[vertex_buffer->count++] = cube_vertices[i * 12 + j * 3 + 2];

			int c = 0;

			if (i == 0)
				c = 0;
			else if (i == 1)
				c = 2;
			else
				c = 4;


			if (j == 0) {
				vertex_data[vertex_buffer->count++] = cube_coords[type][c] * m;
				vertex_data[vertex_buffer->count++] = cube_coords[type][c + 1] * m;
			}
			else if (j == 1) {
				vertex_data[vertex_buffer->count++] = cube_coords[type][c] * m;
				vertex_data[vertex_buffer->count++] = (cube_coords[type][c + 1] + 1) * m;
			}
			else if (j == 2) {
				vertex_data[vertex_buffer->count++] = (cube_coords[type][c] + 1) * m;
				vertex_data[vertex_buffer->count++] = (cube_coords[type][c + 1] + 1) * m;
			}
			else if (j == 3) {
				vertex_data[vertex_buffer->count++] = (cube_coords[type][c] + 1) * m;
				vertex_data[vertex_buffer->count++] = cube_coords[type][c + 1] * m;
			}
		}
	}


	for (int i = 0; i < 6; i++) {
		indice_data[indice_buffer->count++] = i * 4 + vertex_buffer->index;
		indice_data[indice_buffer->count++] = i * 4 + 1 + vertex_buffer->index;
		indice_data[indice_buffer->count++] = i * 4 + 3 + vertex_buffer->index;
		indice_data[indice_buffer->count++] = i * 4 + 2 + vertex_buffer->index;
		indice_data[indice_buffer->count++] = i * 4 + 3 + vertex_buffer->index;
		indice_data[indice_buffer->count++] = i * 4 + 1 + vertex_buffer->index;
	}

	vertex_buffer->index += 24;

}