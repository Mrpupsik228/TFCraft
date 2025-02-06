#pragma once

#include <FastNoise/FastNoise.h>

struct Noise {
    FastNoise::SmartNode<FastNoise::Perlin> perlinNoise;
    FastNoise::SmartNode<FastNoise::Simplex> simplexNoise;
    FastNoise::SmartNode<FastNoise::Value> valueNoise;                                                                       

    int seed;
    float frequency;

    Noise() {
        perlinNoise = FastNoise::New<FastNoise::Perlin>();
        simplexNoise = FastNoise::New<FastNoise::Simplex>();
        valueNoise = FastNoise::New<FastNoise::Value>();
    }

    float GetPerlin(float x, float y) {
        return perlinNoise->GenSingle2D(x * frequency, y * frequency, seed);
    }

    float GetPerlin(float x, float y, float z) {
        return perlinNoise->GenSingle3D(x * frequency, y * frequency, z * frequency, seed);
    }

    float GetSimplex(float x, float y) {
        return simplexNoise->GenSingle2D(x * frequency, y * frequency, seed);
    }

    float GetSimplex(float x, float y, float z) {
        return simplexNoise->GenSingle3D(x * frequency, y * frequency, z * frequency, seed);
    }

    // Get Value noise (2D) - Used instead of WhiteNoise
    float GetWhiteNoise(float x, float y) {
        return valueNoise->GenSingle2D(x * frequency, y * frequency, seed);
    }

    // Get Value noise (3D)
    float GetWhiteNoise(float x, float y, float z) {
        return valueNoise->GenSingle3D(x * frequency, y * frequency, z * frequency, seed);
    }
};
