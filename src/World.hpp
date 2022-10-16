#ifndef WORLD_HPP
#define WORLD_HPP

#include <glm/glm.hpp>

#include <Texture.hpp>
#include <Shader.hpp>
#include <Block.hpp>
#include <VAO.hpp>
#include <VBO.hpp>
#include <EBO.hpp>

#include <time.h>

const glm::vec3 WORLD_SIZE(256, 32, 256);
const uint64_t WORLD_VOLUME = WORLD_SIZE.x * WORLD_SIZE.y * WORLD_SIZE.z;

const size_t DATA_BUFFER_SIZE = (256 * 32 * 256) * 8 * 24;
const size_t INDICES_BUFFER_SIZE = (256 * 32 * 256) * 36;
const size_t ENEMY_DATA_BUFFER_SIZE = 8 * 24;
const size_t ENEMY_INDICES_BUFFER_SIZE = 36;

const glm::vec3 FACES[6] = {
	{1, 0, 0}, //RIGHT
	{0, 1, 0}, //UP
	{0, 0, 1}, //FOWARD
	{-1, 0, 0}, //LEFT
	{0, -1, 0}, //DOWN
	{0, 0, -1} //BACKWARD
};

const float FACES_F[6][3] = {
	{1, 0, 0}, //RIGHT
	{0, 1, 0}, //UP
	{0, 0, 1}, //FOWARD
	{-1, 0, 0}, //LEFT
	{0, -1, 0}, //DOWN
	{0, 0, -1} //BACKWARD
};

class World;
class EnemyObject;

void worldgen_generate(World* world);
void worldgen_generate_npc(World* world);


class EnemyObject {
public:
	struct DataBuffer vertices, indices;
	size_t vertex_count;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	glm::vec3 position;
	World* world;

	void enemy_init(Shader* _shader, World* _world, glm::vec3 origin_position);
	void enemy_prepare();
	void enemy_buffer();
	void enemy_render();
	void enemy_update(glm::vec3 camera_position);
	void enemy_delete();

private:
	Shader* shader;
};

class Mesh {
public:
	World *world;

	struct DataBuffer vertices, indices;
	size_t vertex_count;

	// buffer objects
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	void mesh_init(World* _world);
	void mesh_prepare();
	void mesh_buffer();
	void mesh_render();
	void mesh_destroy();
};

class World {
public:
	bool dirty;
	uint32_t *data;
	Mesh* mesh;

	EnemyObject **enemy_obj;
	size_t max_enemy;
	size_t enemy_count;

	void world_init(Shader* _shader);
	void world_destroy();
	void world_render();
	void world_update(glm::vec3 camera_position);

	void world_add_enemy(EnemyObject* enemy);
	size_t world_pos_to_index(glm::vec3 p);
	void world_set_data(glm::vec3 p, uint32_t data);
	uint32_t world_get_data(glm::vec3 pos);
	bool world_in_bounds(glm::vec3 pos);

	bool exposed(glm::vec3 position);

	Shader* get_shader();
private:
	Shader *shader;
};

static void genTree(World* world, uint32_t x, uint32_t y, uint32_t z);

#endif