#pragma once

#include "chunk.hpp"

class World {
public:
    static const int16_t WORLD_WIDTH = 32;
    static const int16_t WORLD_LENGHT = 32;
    static const int16_t WORLD_HEIGHT = 1;

    static const int16_t CHUNK_WIDTH = Chunk::CHUNK_WIDTH;
    static const int16_t CHUNK_LENGHT = Chunk::CHUNK_LENGHT;
    static const int16_t CHUNK_HEIGHT = Chunk::CHUNK_HEIGHT;

    std::vector<Chunk> chunks;

    void generateWorld(int x, int y, int z);
};