#include "world.hpp"

void World::generateWorld(int x, int y, int z) {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        for (int j = 0; j < WORLD_LENGHT; j++) {
            for (int k = 0; k < WORLD_HEIGHT; k++) {
                Chunk chunk;
                chunk.x = i;
                chunk.y = j;
                chunk.generateMesh();
                chunks.push_back(chunk);
            }
        }
    }
}