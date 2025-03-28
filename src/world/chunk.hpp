#pragma once
#include "./../engine/engine.h"

class Chunk {
public:
    static const int16_t CHUNK_WIDTH = 16;
    static const int16_t CHUNK_LENGHT = 16;
    static const int16_t CHUNK_HEIGHT = 256;

    static int x;
    static int y;

    std::array<int8_t, CHUNK_WIDTH * CHUNK_LENGHT * CHUNK_HEIGHT> blocks;

    BS::Mesh generateMesh();

    int16_t getBlockXYZ(int16_t x, int16_t y, int16_t z) const;

private:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
};
