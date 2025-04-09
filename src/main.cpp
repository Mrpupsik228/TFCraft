#include "engine/global.hpp"
#include "camera.hpp"

#include <vector>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/packing.hpp>

#include "FastNoise.h"

class BlockTextureAtlas {
private:
	static GLuint texture;
public:
	static const size_t NUM_TEXTURES_X = 16, NUM_TEXTURES_Y = 16;
	static const float ITEM_SIZE_X, ITEM_SIZE_Y;

	static void initialize() {
		BlockTextureAtlas::texture = Texture::loadFromFile("./assets/textures/terrain.png", GL_NEAREST, GL_REPEAT);
	}
	static void destroy() {
		Texture::destroy(BlockTextureAtlas::texture);
	}

	static void use() {
		Texture::use(BlockTextureAtlas::texture, 0);
	}

	static float getU(GLuint id) {
		return id % BlockTextureAtlas::NUM_TEXTURES_X / (float)BlockTextureAtlas::NUM_TEXTURES_X;
	}
	static float getV(GLuint id) {
		return id / BlockTextureAtlas::NUM_TEXTURES_X / (float)BlockTextureAtlas::NUM_TEXTURES_Y;
	}
};

GLuint BlockTextureAtlas::texture = 0;

const float BlockTextureAtlas::ITEM_SIZE_X = 1.0f / BlockTextureAtlas::NUM_TEXTURES_X;
const float BlockTextureAtlas::ITEM_SIZE_Y = 1.0f / BlockTextureAtlas::NUM_TEXTURES_Y;

struct BlockFaces {
	GLuint front, back, top, bottom, right, left;

	BlockFaces() {
		this->front = 0;
		this->back = 0;
		this->top = 0;
		this->bottom = 0;
		this->left = 0;
		this->right = 0;
	}
	BlockFaces(GLuint all) {
		this->front = all;
		this->back = all;
		this->top = all;
		this->bottom = all;
		this->right = all;
		this->left = all;
	}
	BlockFaces(GLuint side, GLuint top, GLuint bottom) {
		this->front = side;
		this->back = side;
		this->top = top;
		this->bottom = bottom;
		this->right = side;
		this->left = side;
	}
	BlockFaces(GLuint front, GLuint back, GLuint top, GLuint bottom, GLuint right, GLuint left) {
		this->front = front;
		this->back = back;
		this->top = top;
		this->bottom = bottom;
		this->right = right;
		this->left = left;
	}
};

class Block {
private:
	static std::vector<Block> blocks;
public:
	const BlockFaces blockFaces;

	Block() : blockFaces() {}
	Block(const BlockFaces blockFaces) : blockFaces(blockFaces) {}

	static void initialize() {
		Block::registerBlock(Block());
	}

	static void registerBlock(const Block& block) {
		Block::blocks.push_back(block);
	}

	static Block getBlock(GLuint id) {
		if (id >= Block::blocks.size()) return Block::blocks[0];
		return Block::blocks[id];
	}
};

std::vector<Block> Block::blocks = {};

class Chunk {
private:
	uint8_t* blocks = nullptr;
public:
	static const size_t WIDTH = 16, HEIGHT = 16, LENGTH = 16;
	static const size_t VOLUME = Chunk::WIDTH * Chunk::HEIGHT * Chunk::LENGTH;

	const glm::u64vec3 position;

	Chunk(FastNoise noise, size_t* heightMap, glm::u64vec3 position) : position(position) {
		this->blocks = new uint8_t[Chunk::VOLUME]();

		for (size_t x = 0; x < Chunk::WIDTH; x++) {
			for (size_t z = 0; z < Chunk::LENGTH; z++) {
				size_t worldX = x + position.x * Chunk::WIDTH;
				size_t worldZ = z + position.z * Chunk::LENGTH;

				size_t height = heightMap[x + z * Chunk::WIDTH];
				
				for (size_t y = 0; y < glm::clamp((int64_t)(height - position.y * Chunk::HEIGHT), (int64_t)0, (int64_t)Chunk::HEIGHT); y++) {
					uint8_t block = 1;

					size_t worldY = y + position.y * Chunk::HEIGHT;

					// Bedrock floor case
					if (worldY == 0) {
						block = 5;
					}

					// Cave case
					else if (
						worldY < 24 &&
						noise.GetSimplex(worldX * 0.5f + 483.2384f, worldY * 0.5f + 193.823f, worldZ * 0.5f - 934.232f) >= 0.4f &&
						noise.GetSimplex(worldX * 3.0f + 483.2384f, worldY * 5.0f + 193.823f, worldZ * 3.0f - 934.232f) >= 0.4f
					) {
						block = 0;
					}

					// Sand sphere case
					else if (worldY < 12 && noise.GetPerlin(worldX * 5.0f, worldY * 5.0f, worldZ * 5.0f) >= 0.3f) {
						block = 7;
					}

					// Gravel sphere case
					else if (worldY < 12 && noise.GetPerlin(worldX * 5.0f + 329.2342f, worldY * 5.0f, worldZ * 5.0f - 291.382f) >= 0.2f) {
						block = 6;
					}

					// Grass block ceil case
					else if (worldY >= height - 1) {
						block = 2;
					}

					// Stone block fill case
					else if (worldY <= height - 2 - glm::smoothstep(0.3f, 0.8f, noise.GetWhiteNoise(worldX, worldZ) * 0.5f + 0.5f) * 2.0f) {
						block = 3;
					}

					this->setBlockUnsafe(x, y, z, block);
				}
			}
		}
	}
	~Chunk() {
		delete[] this->blocks;
	}

	void setBlock(size_t x, size_t y, size_t z, uint8_t block) {
		if (x >= Chunk::WIDTH || y >= Chunk::HEIGHT || z >= Chunk::LENGTH) return;
		this->blocks[x + z * Chunk::WIDTH + y * Chunk::WIDTH * Chunk::LENGTH] = block;
	}
	void setBlockUnsafe(size_t x, size_t y, size_t z, uint8_t block) {
		this->blocks[x + z * Chunk::WIDTH + y * Chunk::WIDTH * Chunk::LENGTH] = block;
	}

	uint8_t getBlock(size_t x, size_t y, size_t z) const {
		if (x >= Chunk::WIDTH || y >= Chunk::HEIGHT || z >= Chunk::LENGTH) return 0;
		return this->blocks[x + z * Chunk::WIDTH + y * Chunk::WIDTH * Chunk::LENGTH];
	}
	uint8_t getBlockUnsafe(size_t x, size_t y, size_t z) const {
		return this->blocks[x + z * Chunk::WIDTH + y * Chunk::WIDTH * Chunk::LENGTH];
	}
};

struct NextChunks {
	Chunk *front, *back, *top, *bottom, *right, *left;
	NextChunks(Chunk *front, Chunk *back, Chunk *top, Chunk *bottom, Chunk *right, Chunk *left) {
		this->front = front;
		this->back = back;
		this->top = top;
		this->bottom = bottom;
		this->right = right;
		this->left = left;
	}
};

class ChunkMesh {
private:
	GLuint vaoId = 0, vboId = 0;
	Chunk& chunk;

	std::vector<float> data;

	inline void putVertex(float x, float y, float z, float u, float v, float light) {
		this->data.push_back(x);
		this->data.push_back(y);
		this->data.push_back(z);

		this->data.push_back(u);
		this->data.push_back(v);

		this->data.push_back(light);
	}

	bool readyForGPUUpdate = false;
	bool dirty = false;
public:
	ChunkMesh(Chunk& chunk) : chunk(chunk), data() {
		glGenVertexArrays(1, &this->vaoId);
		glBindVertexArray(this->vaoId);

		glGenBuffers(1, &this->vboId);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboId);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

		glBindVertexArray(0);
	}
	~ChunkMesh() {
		this->destroy();
	}

	void setDirty() {
		this->dirty = true;
	}

	void update(NextChunks nextChunks) {
		if (!this->dirty) return;

		this->readyForGPUUpdate = false;
		this->data.clear();

		for (size_t x = 0; x < Chunk::WIDTH; x++) {
			for (size_t y = 0; y < Chunk::HEIGHT; y++) {
				for (size_t z = 0; z < Chunk::LENGTH; z++) {
					uint8_t blockId = this->chunk.getBlockUnsafe(x, y, z);
					if (blockId == 0) continue;

					Block block = Block::getBlock(blockId);

					if (x <= 0 ? nextChunks.left == nullptr || nextChunks.left->getBlockUnsafe(Chunk::WIDTH - 1, y, z) == 0 : this->chunk.getBlock(x - 1, y, z) == 0) {
						float u0 = BlockTextureAtlas::getU(block.blockFaces.left);
						float v0 = BlockTextureAtlas::getV(block.blockFaces.left);
						float u1 = u0 + BlockTextureAtlas::ITEM_SIZE_X;
						float v1 = v0 + BlockTextureAtlas::ITEM_SIZE_Y;

						float light = 0.7f;
						
						this->putVertex(x,        y,        z, u0, v0, light);
						this->putVertex(x,        y, z + 1.0f, u1, v0, light);
						this->putVertex(x, y + 1.0f,        z, u0, v1, light);

						this->putVertex(x, y + 1.0f, z + 1.0f, u1, v1, light);
						this->putVertex(x, y + 1.0f,        z, u0, v1, light);
						this->putVertex(x,        y, z + 1.0f, u1, v0, light);
					}
					if (x >= Chunk::WIDTH - 1 ? nextChunks.right == nullptr || nextChunks.right->getBlockUnsafe(0, y, z) == 0 : this->chunk.getBlock(x + 1, y, z) == 0) {
						float u0 = BlockTextureAtlas::getU(block.blockFaces.right);
						float v0 = BlockTextureAtlas::getV(block.blockFaces.right);
						float u1 = u0 + BlockTextureAtlas::ITEM_SIZE_X;
						float v1 = v0 + BlockTextureAtlas::ITEM_SIZE_Y;

						float light = 0.9f;

						this->putVertex(x + 1.0f,        y, z + 1.0f, u0, v0, light);
						this->putVertex(x + 1.0f,        y,        z, u1, v0, light);
						this->putVertex(x + 1.0f, y + 1.0f, z + 1.0f, u0, v1, light);

						this->putVertex(x + 1.0f, y + 1.0f,        z, u1, v1, light);
						this->putVertex(x + 1.0f, y + 1.0f, z + 1.0f, u0, v1, light);
						this->putVertex(x + 1.0f,        y,        z, u1, v0, light);
					}

					if (y <= 0 ? nextChunks.bottom == nullptr || nextChunks.bottom->getBlockUnsafe(x, Chunk::HEIGHT - 1, z) == 0 : this->chunk.getBlock(x, y - 1, z) == 0) {
						float u0 = BlockTextureAtlas::getU(block.blockFaces.bottom);
						float v0 = BlockTextureAtlas::getV(block.blockFaces.bottom);
						float u1 = u0 + BlockTextureAtlas::ITEM_SIZE_X;
						float v1 = v0 + BlockTextureAtlas::ITEM_SIZE_Y;
						
						float light = 0.65f;

						this->putVertex(       x, y,        z, u0, v0, light);
						this->putVertex(x + 1.0f, y,        z, u1, v0, light);
						this->putVertex(       x, y, z + 1.0f, u0, v1, light);

						this->putVertex(x + 1.0f, y, z + 1.0f, u1, v1, light);
						this->putVertex(       x, y, z + 1.0f, u0, v1, light);
						this->putVertex(x + 1.0f, y,        z, u1, v0, light);
					}
					if (y >= Chunk::HEIGHT - 1 ? nextChunks.top == nullptr || nextChunks.top->getBlockUnsafe(x, 0, z) == 0 : this->chunk.getBlock(x, y + 1, z) == 0) {
						float u0 = BlockTextureAtlas::getU(block.blockFaces.top);
						float v0 = BlockTextureAtlas::getV(block.blockFaces.top);
						float u1 = u0 + BlockTextureAtlas::ITEM_SIZE_X;
						float v1 = v0 + BlockTextureAtlas::ITEM_SIZE_Y;

						float light = 1.0f;

						this->putVertex(       x, y + 1.0f, z + 1.0f, u0, v0, light);
						this->putVertex(x + 1.0f, y + 1.0f, z + 1.0f, u1, v0, light);
						this->putVertex(       x, y + 1.0f,        z, u0, v1, light);

						this->putVertex(x + 1.0f, y + 1.0f,        z, u1, v1, light);
						this->putVertex(       x, y + 1.0f,        z, u0, v1, light);
						this->putVertex(x + 1.0f, y + 1.0f, z + 1.0f, u1, v0, light);
					}

					if (z <= 0 ? nextChunks.back == nullptr || nextChunks.back->getBlockUnsafe(x, y, Chunk::LENGTH - 1) == 0 : this->chunk.getBlock(x, y, z - 1) == 0) {
						float u0 = BlockTextureAtlas::getU(block.blockFaces.back);
						float v0 = BlockTextureAtlas::getV(block.blockFaces.back);
						float u1 = u0 + BlockTextureAtlas::ITEM_SIZE_X;
						float v1 = v0 + BlockTextureAtlas::ITEM_SIZE_Y;

						float light = 0.75f;

						this->putVertex(x + 1.0f,        y, z, u0, v0, light);
						this->putVertex(       x,        y, z, u1, v0, light);
						this->putVertex(x + 1.0f, y + 1.0f, z, u0, v1, light);

						this->putVertex(       x, y + 1.0f, z, u1, v1, light);
						this->putVertex(x + 1.0f, y + 1.0f, z, u0, v1, light);
						this->putVertex(       x,        y, z, u1, v0, light);
					}
					if (z >= Chunk::LENGTH - 1 ? nextChunks.front == nullptr || nextChunks.front->getBlockUnsafe(x, y, 0) == 0 : this->chunk.getBlock(x, y, z + 1) == 0) {
						float u0 = BlockTextureAtlas::getU(block.blockFaces.front);
						float v0 = BlockTextureAtlas::getV(block.blockFaces.front);
						float u1 = u0 + BlockTextureAtlas::ITEM_SIZE_X;
						float v1 = v0 + BlockTextureAtlas::ITEM_SIZE_Y;

						float light = 0.95f;

						this->putVertex(       x,        y, z + 1.0f, u0, v0, light);
						this->putVertex(x + 1.0f,        y, z + 1.0f, u1, v0, light);
						this->putVertex(       x, y + 1.0f, z + 1.0f, u0, v1, light);

						this->putVertex(x + 1.0f, y + 1.0f, z + 1.0f, u1, v1, light);
						this->putVertex(       x, y + 1.0f, z + 1.0f, u0, v1, light);
						this->putVertex(x + 1.0f,        y, z + 1.0f, u1, v0, light);
					}
				}
			}
		}

		this->dirty = false;
		this->readyForGPUUpdate = true;
	}
	void sendToGPU() {
		if (!this->readyForGPUUpdate) return;

		glBindVertexArray(this->vaoId);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
		glBufferData(GL_ARRAY_BUFFER, this->data.size() * sizeof(float), this->data.data(), GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
		
		this->readyForGPUUpdate = false;
	}

	void render() const {
		glBindVertexArray(this->vaoId);
		glDrawArrays(GL_TRIANGLES, 0, this->data.size() / 6);
	}
	void destroy() {
		glDeleteVertexArrays(1, &this->vaoId);
		glDeleteBuffers(1, &this->vboId);

		this->data.clear();

		this->vaoId = 0;
		this->vboId = 0;
	}
};

class World {
private:
	Chunk** chunks = nullptr;
	ChunkMesh** chunkMeshes = nullptr;
public:
	const static size_t WIDTH = 32, HEIGHT = 6;
	size_t seed;

	World() {
		this->seed = rand();

		const size_t volume = World::WIDTH * World::WIDTH * World::HEIGHT;

		this->chunks = new Chunk*[volume]();
		this->chunkMeshes = new ChunkMesh*[volume]();

		FastNoise noise = FastNoise(seed);
		size_t* heightMap = new size_t[Chunk::WIDTH * Chunk::WIDTH];

		for (size_t x = 0; x < World::WIDTH; x++) {
			for (size_t z = 0; z < World::WIDTH; z++) {
				for (size_t localX = 0; localX < Chunk::WIDTH; localX++) {
					for (size_t localZ = 0; localZ < Chunk::LENGTH; localZ++) {
						size_t worldX = localX + x * Chunk::WIDTH;
						size_t worldZ = localZ + z * Chunk::LENGTH;

						float mountainHeight = glm::smoothstep(0.3f, 0.8f, noise.GetPerlin(worldX, worldZ) * 0.5f + 0.5f) * 64.0f;
						mountainHeight += glm::smoothstep(0.3f, 0.8f, noise.GetPerlin(worldX * 5.0f, worldZ * 5.0f) * 0.5f + 0.5f) * 8.0f;
						mountainHeight += 4.0f;

						float plainHeight = glm::smoothstep(0.3f, 0.8f, noise.GetPerlin(worldX * 4.0f, worldZ * 4.0f) * 0.5f + 0.5f) * 4.0f;
						plainHeight += glm::smoothstep(0.3f, 0.8f, noise.GetPerlin(worldX, worldZ) * 0.5f + 0.5f) * 13.0f;
						plainHeight += 4.0f;

						float height = glm::mix(
							plainHeight,
							mountainHeight,
							glm::smoothstep(0.0f, 0.5f, glm::smoothstep(0.3f, 0.8f, noise.GetPerlin(worldX * 0.3f, worldZ * 0.3f) * 0.5f + 0.5f))
						);

						heightMap[localX + localZ * Chunk::WIDTH] = height;
					}
				}

				for (size_t y = 0; y < World::HEIGHT; y++) {
					size_t id = x + z * World::WIDTH + y * World::WIDTH * World::WIDTH;

					this->chunks[id] = new Chunk(noise, heightMap, glm::u64vec3(x, y, z));
					this->chunkMeshes[id] = new ChunkMesh(*this->chunks[id]);
					this->chunkMeshes[id]->setDirty();
				}

			}
		}

		delete[] heightMap;
	}
	~World() {
		this->destroy();
	}

	void update() {
		for (size_t x = 0; x < World::WIDTH; x++) {
			for (size_t y = 0; y < World::HEIGHT; y++) {
				for (size_t z = 0; z < World::WIDTH; z++) {
					size_t id = x + z * World::WIDTH + y * World::WIDTH * World::WIDTH;
					this->chunkMeshes[id]->update(NextChunks(
						z >= World::WIDTH - 1 ? nullptr : this->chunks[x + (z + 1) * World::WIDTH + y * World::WIDTH * World::WIDTH],
						z <= 0 ? nullptr : this->chunks[x + (z - 1) * World::WIDTH + y * World::WIDTH * World::WIDTH],
						y >= World::HEIGHT - 1 ? nullptr : this->chunks[x + z * World::WIDTH + (y + 1) * World::WIDTH * World::WIDTH],
						y <= 0 ? nullptr : this->chunks[x + z * World::WIDTH + (y - 1) * World::WIDTH * World::WIDTH],
						x >= World::WIDTH - 1 ? nullptr : this->chunks[(x + 1) + z * World::WIDTH + y * World::WIDTH * World::WIDTH],
						x <= 0 ? nullptr : this->chunks[(x - 1) + z * World::WIDTH + y * World::WIDTH * World::WIDTH]
					));
					this->chunkMeshes[id]->sendToGPU();
				}
			}
		}
	}
	void render(const Shader& shader, const glm::mat4& projectionView) {
		BlockTextureAtlas::use();
		shader.setInt("u_ColorSampler", 0);

		for (size_t x = 0; x < World::WIDTH; x++) {
			for (size_t y = 0; y < World::HEIGHT; y++) {
				for (size_t z = 0; z < World::WIDTH; z++) {
					size_t id = x + z * World::WIDTH + y * World::WIDTH * World::WIDTH;
					if (this->chunkMeshes[id] != nullptr) {
						shader.setMatrix4("u_MVPMatrix", projectionView * glm::translate(glm::mat4(1.0f), glm::vec3(x * Chunk::WIDTH, y * Chunk::HEIGHT, z * Chunk::LENGTH)));
						this->chunkMeshes[id]->render();
					}
				}
			}
		}
	}
	void destroy() {
		for (size_t i = 0; i < World::WIDTH * World::WIDTH * World::HEIGHT; i++) {
			delete this->chunks[i];
			delete this->chunkMeshes[i];
		}

		delete[] this->chunks;
		delete[] this->chunkMeshes;
	}

	Chunk* getChunk(size_t x, size_t y, size_t z) const {
		if (x >= World::WIDTH || y >= World::HEIGHT || z >= World::WIDTH) return nullptr;
		return this->chunks[x + z * World::WIDTH + y * World::WIDTH * World::WIDTH];
	}
	ChunkMesh* getChunkMesh(size_t x, size_t y, size_t z) const {
		if (x >= World::WIDTH || y >= World::HEIGHT || z >= World::WIDTH) return nullptr;

		size_t id = x + z * World::WIDTH + y * World::WIDTH * World::WIDTH;
		ChunkMesh* mesh = this->chunkMeshes[id];

		if (mesh == nullptr) {
			mesh = new ChunkMesh(*this->getChunk(x, y, z));
		}

		return mesh;
	}

	uint8_t getBlock(size_t x, size_t y, size_t z) const {
		size_t chunkX = x / Chunk::WIDTH, chunkY = y / Chunk::HEIGHT, chunkZ = z / Chunk::LENGTH;
		// if (x < 0) chunkX--;
		// if (y < 0) chunkY--;
		// if (z < 0) chunkZ--;

		Chunk* chunk = this->getChunk(chunkX, chunkY, chunkZ);
		if (chunk == nullptr) return 0;

		return chunk->getBlockUnsafe(x - chunkX * Chunk::WIDTH, y - chunkY * Chunk::HEIGHT, z - chunkZ * Chunk::LENGTH);
	}
	void setBlock(size_t x, size_t y, size_t z, uint8_t block) const {
		size_t chunkX = x / Chunk::WIDTH, chunkY = y / Chunk::HEIGHT, chunkZ = z / Chunk::LENGTH;
		// if (x < 0) chunkX--;
		// if (y < 0) chunkY--;
		// if (z < 0) chunkZ--;

		Chunk* chunk = this->getChunk(chunkX, chunkY, chunkZ);
		if (chunk == nullptr) return;

		size_t localX = x - chunkX * Chunk::WIDTH;
		size_t localY = y - chunkY * Chunk::HEIGHT;
		size_t localZ = z - chunkZ * Chunk::LENGTH;

		chunk->setBlockUnsafe(localX, localY, localZ, block);
		
		ChunkMesh* mesh = this->getChunkMesh(chunkX, chunkY, chunkZ);
		if (mesh != nullptr) mesh->setDirty();
		
		if (localX <= 0) {
			mesh = this->getChunkMesh(chunkX - 1, chunkY, chunkZ);
			if (mesh != nullptr) mesh->setDirty();
		}
		if (localX >= Chunk::WIDTH - 1) {
			mesh = this->getChunkMesh(chunkX + 1, chunkY, chunkZ);
			if (mesh != nullptr) mesh->setDirty();
		}

		if (localY <= 0) {
			mesh = this->getChunkMesh(chunkX, chunkY - 1, chunkZ);
			if (mesh != nullptr) mesh->setDirty();
		}
		if (localY >= Chunk::HEIGHT - 1) {
			mesh = this->getChunkMesh(chunkX, chunkY + 1, chunkZ);
			if (mesh != nullptr) mesh->setDirty();
		}

		if (localZ <= 0) {
			mesh = this->getChunkMesh(chunkX, chunkY, chunkZ - 1);
			if (mesh != nullptr) mesh->setDirty();
		}
		if (localZ >= Chunk::LENGTH - 1) {
			mesh = this->getChunkMesh(chunkX, chunkY, chunkZ + 1);
			if (mesh != nullptr) mesh->setDirty();
		}
	}
};

uint8_t raycast(const World& world, const glm::vec3& origin, const glm::vec3& direction, float maxDistance, glm::vec3& normal, glm::vec3& endPosition) {
	endPosition = glm::floor(origin);
	normal = glm::vec3();

	glm::vec3 deltaDist = glm::abs(glm::vec3(1.0f) / direction);
	glm::vec3 step = glm::vec3(
		direction.x > 0 ? 1 : -1,
		direction.y > 0 ? 1 : -1,
		direction.z > 0 ? 1 : -1
	);
	glm::vec3 sideDist = glm::vec3(
		(direction.x > 0 ? (endPosition.x + 1 - origin.x) : (origin.x - endPosition.x)) * deltaDist.x,
		(direction.y > 0 ? (endPosition.y + 1 - origin.y) : (origin.y - endPosition.y)) * deltaDist.y,
		(direction.z > 0 ? (endPosition.z + 1 - origin.z) : (origin.z - endPosition.z)) * deltaDist.z
	);

	float distanceTraveled = 0.0f;
	while (distanceTraveled < maxDistance) {
		uint8_t block = world.getBlock(endPosition.x, endPosition.y, endPosition.z);
		if (block != 0) {
			return block;
		}

		if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
			distanceTraveled = sideDist.x;
			sideDist.x += deltaDist.x;
			endPosition.x += step.x;

			normal = glm::vec3(-step.x, 0.0f, 0.0f);
		} else if (sideDist.y < sideDist.z) {
			distanceTraveled = sideDist.y;
			sideDist.y += deltaDist.y;
			endPosition.y += step.y;

			normal = glm::vec3(0.0f, -step.y, 0.0f);
		} else {
			distanceTraveled = sideDist.z;
			sideDist.z += deltaDist.z;
			endPosition.z += step.z;

			normal = glm::vec3(0.0f, 0.0f, -step.z);
		}
	}

	return 0;
}

int main() {
	Window::create(1920, 1080, "TFCraft");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Shader shader = Shader("./assets/shaders/terrain.vert", "./assets/shaders/terrain.frag");

	BlockTextureAtlas::initialize();

	Block::initialize();
	Block::registerBlock(Block(BlockFaces(1))); // dirt
	Block::registerBlock(Block(BlockFaces(3, 2, 1))); // grass
	Block::registerBlock(Block(BlockFaces(4))); // stone
	Block::registerBlock(Block(BlockFaces(5))); // cobblestone
	Block::registerBlock(Block(BlockFaces(6))); // bedrock
	Block::registerBlock(Block(BlockFaces(7))); // gravel
	Block::registerBlock(Block(BlockFaces(8))); // sand

	Camera camera = Camera(90.0f, 0.01f, 500.0f);
	camera.position.x = 10.0f;
	camera.position.y = 10.0f;
	camera.position.z = 30.0f;

	World* world = new World();

	Timer timer = Timer();
	Input::grabCursor();

	float fpsTimer = 0.0f;
	int fps = 0;

	while (Window::isRunning()) {
		Window::pollEvents();
		timer.update();

		fpsTimer += timer.getDeltaTime();
		fps++;

		if (fpsTimer >= 1.0f) {
			std::cout << "FPS: " << fps << '\n';
			fpsTimer = 0.0f;
			fps = 0;
		}

		if (Input::isKeyJustPressed(GLFW_KEY_R)) {
			delete world;
			world = new World();
		}

		if (Input::isKeyJustPressed(GLFW_KEY_ESCAPE)) {
			Input::toggleCursor();
		}
		if (!Input::isCursorReleased()) {
			camera.fly(timer, Input::isKeyPressed(GLFW_KEY_LEFT_CONTROL) ? 16.0f : 4.31f, 0.1f);
		}
		camera.update();

		if (Input::isMouseJustPressed(GLFW_MOUSE_BUTTON_LEFT)) {
			glm::vec3 normal, endPosition;
			uint8_t block = raycast(*world, camera.position, camera.getFront(), 64.0f, normal, endPosition);

			if (block != 0) {
				int64_t radius = 1;
				for (int64_t x = -radius; x <= radius; x++) {
					for (int64_t y = -radius; y <= radius; y++) {
						for (int64_t z = -radius; z <= radius; z++) {
							if (glm::length(glm::vec3(x, y, z)) >= radius) continue;
							world->setBlock(endPosition.x + x, endPosition.y + y, endPosition.z + z, 0);
						}
					}
				}
			}
		}
		if (Input::isMouseJustPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			glm::vec3 normal, endPosition;
			uint8_t block = raycast(*world, camera.position, camera.getFront(), 64.0f, normal, endPosition);

			if (block != 0) {
				int64_t radius = 1;
				for (int64_t x = -radius; x <= radius; x++) {
					for (int64_t y = -radius; y <= radius; y++) {
						for (int64_t z = -radius; z <= radius; z++) {
							if (glm::length(glm::vec3(x, y, z)) >= radius) continue;
							world->setBlock(endPosition.x + normal.x + x, endPosition.y + normal.y + y, endPosition.z + normal.z + z, 1);
						}
					}
				}
			}
		}

		world->update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		world->render(shader, camera.getProjectionView());
		shader.drop();

		Window::swapBuffers();
	}

	delete world;

	BlockTextureAtlas::destroy();
	Window::destroy();
}