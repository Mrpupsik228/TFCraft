#include "chunk.hpp"


Chunk::Chunk() {}
Chunk::~Chunk() {}

BS::Mesh Chunk::generate() {
    std::vector<float> vertices = {};

    for (uint16_t x = 0; x < WIDTH; x++) {
        for (uint16_t y = 0; y < HEIGHT; y++) {
            for (uint16_t z = 0; z < LENGTH; z++) {
                uint8_t block = getBlockXYZ(x, y, z);
                if (block == 0) {
                    continue;
                }
                if (getBlockXYZ(x, y, z + 1) == 0) {
                    vertices.reserve(vertices.size() + 18);
                
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);
                }
                if(getBlockXYZ(x, y, z - 1) == 0) {
                    vertices.reserve(vertices.size() + 18);
                
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);

                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);

                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                }
                if(getBlockXYZ(x, y - 1, z) == 0) {
                    vertices.reserve(vertices.size() + 18);
                
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);

                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);
                }
                if(getBlockXYZ(x, y + 1, z) == 0) {
                    vertices.reserve(vertices.size() + 18);
                
                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);

                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);
                }
                if(getBlockXYZ(x - 1, y, z) == 0) {
                    vertices.reserve(vertices.size() + 18);
                
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);

                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);
                }
                if(getBlockXYZ(x + 1, y, z) == 0) {
                    vertices.reserve(vertices.size() + 18);
                
                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x + 1);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z + 1);

                    vertices.push_back(x + 1);
                    vertices.push_back(y + 1);
                    vertices.push_back(z);
                }
            }
        }
    }

    return BS::Mesh(BS::VertexBuffer(vertices, 3), { }, BS::Mesh::TRIANGLES);
}

void Chunk::setBlock(uint16_t x, uint16_t y, uint16_t z, uint8_t block) {
    data[x + (y * WIDTH) + (z * WIDTH * HEIGHT)] = block;
}

uint8_t Chunk::getBlockXYZ(uint16_t x, uint16_t y, uint16_t z) {
        if (x >= WIDTH || y >= HEIGHT || z >= LENGTH) {
        return 0;
    }
    return data[x + (y * WIDTH) + (z * WIDTH * HEIGHT)];
}
