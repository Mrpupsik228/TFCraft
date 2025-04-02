#pragma once
#include <cstdint>
#include "engine/engine.h"

class Chunk {
private:
    static const uint16_t HEIGHT = 256, WIDTH = 16, LENGTH = 16;
    std::array<uint8_t, HEIGHT*WIDTH*LENGTH> data;

public:
    Chunk();
    ~Chunk();

    BS::Mesh generate(); 

    void setBlock(uint16_t x, uint16_t y, uint16_t z, uint8_t block);

    uint8_t getBlockXYZ(uint16_t x, uint16_t y, uint16_t z);
};
