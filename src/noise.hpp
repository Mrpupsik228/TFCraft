#pragma once

#include <FastNoise/FastNoise.h>

struct Noise {
    FastNoise::SmartNode<FastNoise::Perlin> perlinNoise;
    FastNoise::SmartNode<FastNoise::Simplex> simplexNoise;
    FastNoise::SmartNode<FastNoise::Value> valueNoise;  // White noise alternative

    int seed;

    Noise() {
        perlinNoise = FastNoise::New<FastNoise::Perlin>();
        simplexNoise = FastNoise::New<FastNoise::Simplex>();
        valueNoise = FastNoise::New<FastNoise::Value>();  // Using Value noise as WhiteNoise alternative
    }

    // Get Perlin noise (2D)
    float GetPerlin(float x, float y) {
        return perlinNoise->GenSingle2D(x, y, seed);
    }

    // Get Perlin noise (3D)
    float GetPerlin(float x, float y, float z) {
        return perlinNoise->GenSingle3D(x, y, z, seed);
    }

    // Get Simplex noise (2D)
    float GetSimplex(float x, float y) {
        return simplexNoise->GenSingle2D(x, y, seed);
    }

    // Get Simplex noise (3D)
    float GetSimplex(float x, float y, float z) {
        return simplexNoise->GenSingle3D(x, y, z, seed);
    }

    // Get Value noise (2D) - Used instead of WhiteNoise
    float GetWhiteNoise(float x, float y) {
        return valueNoise->GenSingle2D(x, y, seed);
    }

    // Get Value noise (3D)
    float GetWhiteNoise(float x, float y, float z) {
        return valueNoise->GenSingle3D(x, y, z, seed);
    }
};
