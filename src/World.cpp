#include "World.hpp"


#define SRAND(seed) srand(seed)
#define RAND(min, max) ((rand() % (max - min + 1)) + min)
#define RANDCHANCE(chance) ((RAND(0, 100000) / 100000.0) <= chance)

//ENEMY
void EnemyObject::enemy_init(Shader* _shader, World* _world, glm::vec3 origin_position) {
	this->shader = _shader;
	this->world = _world;
	this->position = origin_position;

	this->vao = VAO::vao_create();
	this->vbo = VBO::vbo_create();
	this->ebo = EBO::ebo_create();

	this->vertices.capacity = ENEMY_DATA_BUFFER_SIZE;
	this->indices.capacity = ENEMY_INDICES_BUFFER_SIZE;

	struct DataBuffer *buffers[2] = {
	&this->vertices, &this->indices
	};

	void* global_data[2] = { (void*)((float*)malloc(ENEMY_DATA_BUFFER_SIZE * sizeof(float))),
							 (void*)((int*)malloc(ENEMY_INDICES_BUFFER_SIZE * sizeof(int))) };

	for (size_t i = 0; i < 2; i++) {
		struct DataBuffer *buffer = buffers[i];
		buffer->count = 0;
		buffer->index = 0;
		buffer->data = global_data[i];
	}
}

void EnemyObject::enemy_prepare() {
	this->vertex_count = 0;
	this->vertices.count = 0;
	this->vertices.index = 0;
	this->indices.count = 0;
	this->indices.index = 0;
}

void EnemyObject::enemy_buffer() {
	add_cube_buffer(&this->vertices, &this->indices, this->position, BLOCKTYPE_SAND, 0.5);

	VAO::vao_bind(this->vao);
	VBO::vbo_buffer(this->vbo, (float*)this->vertices.data, this->vertices.count * sizeof(float));
	EBO::ebo_buffer(this->ebo, (GLuint*)this->indices.data, this->indices.count * sizeof(GLuint));

	VAO::LinkAttrib(this->vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO::LinkAttrib(this->vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO::LinkAttrib(this->vbo, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO::vao_unbind();
	VBO::vbo_unbind();
	EBO::ebo_unbind();
}

void EnemyObject::enemy_render() {
	VBO::vbo_bind(this->vbo);
	VAO::vao_bind(this->vao);
	glDrawElements(GL_TRIANGLES, this->indices.count, GL_UNSIGNED_INT, 0);
	VBO::vbo_unbind();
	VAO::vao_unbind();
}

void EnemyObject::enemy_update(glm::vec3 camera_position) {
	float speed = 0.005f;
	glm::vec3 orientation = camera_position - this->position;
	
	this->position += speed * orientation;

	this->enemy_prepare();
	this->enemy_buffer();
}

void EnemyObject::enemy_delete() {
	VAO::vao_delete(this->vao);
	VBO::vbo_delete(this->vbo);
	EBO::ebo_delete(this->ebo);

	free(this->vertices.data);
	free(this->indices.data);
}

// globaldata buffers
struct {
	float vertices[DATA_BUFFER_SIZE];
	int indices[INDICES_BUFFER_SIZE];
} global_buffers;

// Mesh

void Mesh::mesh_init(World* _world) {
	this->world = _world;

	this->vao = VAO::vao_create();
	this->vbo = VBO::vbo_create();
	this->ebo = EBO::ebo_create();

	// Set buffer capacities
	this->vertices.capacity = DATA_BUFFER_SIZE;
	this->indices.capacity = INDICES_BUFFER_SIZE;
}

void Mesh::mesh_prepare() {
	this->vertex_count = 0;

	struct DataBuffer *buffers[2] = {
		&this->vertices, &this->indices
	};

	void* global_data[2] = { global_buffers.vertices,global_buffers.indices };

	for (size_t i = 0; i < 2; i++) {
		struct DataBuffer *buffer = buffers[i];
		buffer->count = 0;
		buffer->index = 0;
		buffer->data = global_data[i];
	}
}

void Mesh::mesh_buffer() {
	for (float x = 0; x < WORLD_SIZE.x; x++) {
		for (float y = 0; y < WORLD_SIZE.y; y++) {
			for (float z = 0; z < WORLD_SIZE.z; z++) {
				glm::vec3 pos = { x,y,z };
				uint32_t data = this->world->world_get_data(pos);
				if (data != 0) {
					if (this->world->exposed(pos)) {
						add_cube_buffer(&this->vertices, &this->indices, pos, data, 0.5);
					}
				}
			}
		}
	}

	VAO::vao_bind(this->vao);
	VBO::vbo_buffer(this->vbo, (float*)this->vertices.data, this->vertices.count * sizeof(float));
	EBO::ebo_buffer(this->ebo, (GLuint*)this->indices.data, this->indices.count * sizeof(GLuint));

	VAO::LinkAttrib(this->vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO::LinkAttrib(this->vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO::LinkAttrib(this->vbo, 2, 3, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	VAO::vao_unbind();
	VBO::vbo_unbind();
	EBO::ebo_unbind();
}

void Mesh::mesh_render() {
	VBO::vbo_bind(this->vbo);
	VAO::vao_bind(this->vao);
	glDrawElements(GL_TRIANGLES, this->indices.count, GL_UNSIGNED_INT, 0);
	VBO::vbo_unbind();
	VAO::vao_unbind();
}

void Mesh::mesh_destroy() {
	VAO::vao_delete(this->vao);
	VBO::vbo_delete(this->vbo);
	EBO::ebo_delete(this->ebo);
}


// WORLD
void World::world_init(Shader* _shader) {
	this->shader = _shader;

	this->dirty = true;
	this->data = (uint32_t*)calloc(1, WORLD_VOLUME * sizeof(uint32_t));

	this->mesh = (Mesh*)malloc(sizeof(class Mesh));
	this->mesh->mesh_init(this);

	this->max_enemy = 10;
	this->enemy_count = 0;
	
	this->enemy_obj = (EnemyObject**) calloc(1, this->max_enemy * sizeof(class EnemyObject*));
	//worldgen_generate_npc(this);
	worldgen_generate(this);
}


void World::world_add_enemy(EnemyObject* enemy) {
	this->enemy_obj[this->enemy_count] = enemy;
	this->enemy_count++;
}

size_t World::world_pos_to_index(glm::vec3 p) {
	return p.x * WORLD_SIZE.y * WORLD_SIZE.z + p.y * WORLD_SIZE.z + p.z;
}

void World::world_set_data(glm::vec3 p, uint32_t data) {
	uint64_t index = this->world_pos_to_index(p);

	if (index < WORLD_VOLUME)
		this->data[this->world_pos_to_index(p)] = data;
}

// returns true if pos is within chunk boundaries
bool World::world_in_bounds(glm::vec3 pos) {
	return pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
		pos.x < WORLD_SIZE.x && pos.y < WORLD_SIZE.y && pos.z < WORLD_SIZE.z;
}

uint32_t World::world_get_data(glm::vec3 pos) {
	if (this->world_in_bounds(pos))
		return this->data[this->world_pos_to_index(pos)];
	return 0;
}

bool World::exposed(glm::vec3 position) {
	for (int i = 0; i < 6; i++) {
		glm::vec3 t_position = position + FACES[i];
		uint32_t data = this->world_get_data(t_position);

		if (data == 0) {
			return true;
		}
	}
	return false;
}

void World::world_destroy() {
	free(this->data);

	this->mesh->mesh_destroy();
	free(this->mesh);

	
	for (size_t i = 0; i < this->enemy_count; i++) {
		EnemyObject* enemy = this->enemy_obj[i];
		enemy->enemy_delete();
		free(enemy);
	}
	//free(this->enemy_obj);

}

void World::world_render() {
	if (this->dirty) {
		this->mesh->mesh_prepare();
		this->mesh->mesh_buffer();

		this->dirty = false;
	}

	this->mesh->mesh_render();

	for (size_t i = 0; i < this->enemy_count; i++)
		this->enemy_obj[i]->enemy_render();
}

void World::world_update(glm::vec3 camera_position) {
	for (size_t i = 0; i < this->enemy_count; i++)
		this->enemy_obj[i]->enemy_update(camera_position);
}


void worldgen_generate_npc(World* world) {
	EnemyObject* enemy = (EnemyObject*) malloc(sizeof(class EnemyObject));
	enemy->enemy_init(world->get_shader(), world, glm::vec3(WORLD_SIZE.x / 2 - 5, 10.0f, WORLD_SIZE.z / 2));
	enemy->enemy_prepare();
	enemy->enemy_buffer();

	world->world_add_enemy(enemy);

	EnemyObject* enemy2 = (EnemyObject*)malloc(sizeof(class EnemyObject));
	enemy2->enemy_init(world->get_shader(), world, glm::vec3(WORLD_SIZE.x / 2 - 5, 10.0f, WORLD_SIZE.z / 2 - 10));
	enemy2->enemy_prepare();
	enemy2->enemy_buffer();

	world->world_add_enemy(enemy2);
}

void worldgen_generate(World* world) {
	int n = 14;
	int y = 4;
	uint32_t choice[2] = { BLOCKTYPE_GRASS, BLOCKTYPE_STONE };

	for (int x = 0; x < WORLD_SIZE.x; x++) {
		for (int z = 0; z < WORLD_SIZE.z; z++) {
			if (x == 0 || x == WORLD_SIZE.x - 1 || z == 0 || z == WORLD_SIZE.z - 1) {
				for (int dy = -4; dy < 10; dy++) {
					world->world_set_data(glm::vec3(x, y + dy, z), BLOCKTYPE_STONE);
				}
			}
			else {
				for (int dy = -4; dy < -1; dy++) {
					world->world_set_data(glm::vec3(x, y + dy, z), BLOCKTYPE_DIRT);
				}

				world->world_set_data(glm::vec3(x, y, z), BLOCKTYPE_GRASS);
			}
		}
	}

	int o = 10;

	for (int _ = 0; _ < 120; _++) {
		int a = RAND(o, (int)WORLD_SIZE.x - o);
		int b = RAND(o, (int)WORLD_SIZE.x - o);
		int c = 5;
		int h = RAND(1, 6);
		int s = RAND(4, 8);
		int d = 1;
		int t = RAND(0, 1);
		for (int yh = 5; yh < 5 + h; yh++) {
			for (int xh = a - s; xh < a + s + 1; xh++) {
				for (int zh = b - s; zh < b + s + 1; zh++) {
					if ((xh - a) * (xh - a) + (zh - b) * (zh - b) > (s + 1) * (s + 1))
						continue;
					if ((xh - 0) * (xh - 0) + (zh - 0) * (zh - 0) < 25)
						continue;
					world->world_set_data(glm::vec3(xh, yh, zh), choice[t]);
				}
			}
			s -= d;
		}
	}

	for (int xb = 0; xb < 256; xb += 16) {
		for (int zb = 0; zb < 256; zb += 16) {
			int num_tree = RAND(8, 32);
			for (int __ = 0; __ < num_tree; __++) {
				int xy = RAND(xb, xb + 15);
				int yy = RAND(5, 10);
				int zy = RAND(zb, zb + 15);
				genTree(world, xy, yy, zy);
			}
		}
	}
}

void genTree(World* world, uint32_t x, uint32_t y, uint32_t z) {
	uint32_t under = world->world_get_data(glm::vec3(x, y - 1, z));
	uint32_t pos = world->world_get_data(glm::vec3(x, y, z));
	if ((under != BLOCKTYPE_GRASS && under != BLOCKTYPE_DIRT) || pos != 0) {
		return;
	}

	uint32_t h = RAND(3, 5);

	for (uint32_t yy = y; yy <= (y + h); yy++) {
		world->world_set_data(glm::vec3(x, yy, z), BLOCKTYPE_WOOD);
	}

	uint32_t lh = RAND(2, 3);

	for (uint32_t xx = (x - 2); xx <= (x + 2); xx++) {
		for (uint32_t zz = (z - 2); zz <= (z + 2); zz++) {
			for (uint32_t yy = (y + h); yy <= (y + h + 1); yy++) {
				uint32_t c = 0;
				c += xx == (x - 2) || xx == (x + 2);
				c += zz == (z - 2) || zz == (z + 2);

				bool corner = x == 2;
				if ((!(xx == x && zz == z) || yy > (y + h)) &&
					!(corner && yy == (y + h + 1) && RANDCHANCE(0.4))) {
					world->world_set_data(glm::vec3(xx, yy, zz), BLOCKTYPE_LEAVE);
				}
			}
		}
	}

	for (uint32_t xx = (x - 1); xx <= (x + 1); xx++) {
		for (uint32_t zz = (z - 1); zz <= (z + 1); zz++) {
			for (uint32_t yy = (y + h + 2); yy <= (y + h + lh); yy++) {
				uint32_t c = 0;
				c += xx == (x - 1) || xx == (x + 1);
				c += zz == (z - 1) || zz == (z + 1);
				bool corner = c == 2;

				if (!(corner && yy == (y + h + lh) && RANDCHANCE(0.8))) {
					world->world_set_data(glm::vec3(xx, yy, zz), BLOCKTYPE_LEAVE);
				}
			}
		}
	}
}

Shader* World::get_shader() {
	return this->shader;
}