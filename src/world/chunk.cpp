#include "chunk.hpp"

int Chunk::x = 0;
int Chunk::y = 0;

int16_t Chunk::getBlockXYZ(int16_t x, int16_t y, int16_t z) const {
    return x + (z * CHUNK_WIDTH) + (y * CHUNK_WIDTH * CHUNK_LENGHT);
}

BS::Mesh Chunk::generateMesh() {
    for (int16_t x = 0; x < CHUNK_WIDTH; x++) {
        for (int16_t y = 0; y < CHUNK_HEIGHT; y++) {
            for (int16_t z = 0; z < CHUNK_LENGHT; z++) {
                if (blocks[getBlockXYZ(x, y, z)] == 0) {
                    continue;
                }

                if (blocks[getBlockXYZ(x - 1, y, z)] == 0) {
                    // Add face to the left
                    vertices.reserve(vertices.size() + 18); // 6 vertices × 3 components (x, y, z)
                    normals.reserve(normals.size() + 18);  // 6 normals × 3 components (x, y, z)
                    uvs.reserve(uvs.size() + 12);          // 6 UVs × 2 components (u, v)

                    // Vertices
                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z);       // Triangle 1
                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z);
                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);

                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z);       // Triangle 2
                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);
                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z + 1);

                    // Normals (left face normal: -1.0f, 0.0f, 0.0f)
                    normals.emplace_back(-1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(-1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(-1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(-1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(-1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(-1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);

                    // UVs
                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(0.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);

                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(0.0f);
                }
                
                if (blocks[getBlockXYZ(x + 1, y, z)] == 0) {
                    // Add face to the right
                    vertices.reserve(vertices.size() + 18);
                    normals.reserve(normals.size() + 18);
                    uvs.reserve(uvs.size() + 12);

                    // Vertices
                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z);       // Triangle 1
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z);
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);

                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z);       // Triangle 2
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);
                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z + 1);

                    // Normals (right face normal: 1.0f, 0.0f, 0.0f)
                    normals.emplace_back(1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(1.0f); normals.emplace_back(0.0f); normals.emplace_back(0.0f);

                    // UVs
                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(0.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);

                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(0.0f);
                }
                
                if (blocks[getBlockXYZ(x, y - 1, z)] == 0) {
                    // Add face to the bottom
                    vertices.reserve(vertices.size() + 18);
                    normals.reserve(normals.size() + 18);
                    uvs.reserve(uvs.size() + 12);

                    // Vertices
                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z);           // Triangle 1
                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z);
                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z + 1);

                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z);           // Triangle 2
                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z + 1);
                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z + 1);

                    // Normals (bottom face normal: 0.0f, -1.0f, 0.0f)
                    normals.emplace_back(0.0f); normals.emplace_back(-1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(-1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(-1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(-1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(-1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(-1.0f); normals.emplace_back(0.0f);

                    // UVs
                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);

                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(0.0f); uvs.emplace_back(1.0f);
                }
                
                if (blocks[getBlockXYZ(x, y + 1, z)] == 0) {
                    // Add face to the top
                    vertices.reserve(vertices.size() + 18);
                    normals.reserve(normals.size() + 18);
                    uvs.reserve(uvs.size() + 12);

                    // Vertices
                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z);       // Triangle 1
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z);
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);

                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z);       // Triangle 2
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);
                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);

                    // Normals (top face normal: 0.0f, 1.0f, 0.0f)
                    normals.emplace_back(0.0f); normals.emplace_back(1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(1.0f); normals.emplace_back(0.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(1.0f); normals.emplace_back(0.0f);

                    // UVs
                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);

                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(0.0f); uvs.emplace_back(1.0f);
                }
                
                if (blocks[getBlockXYZ(x, y, z - 1)] == 0) {
                    // Add face to the back
                    vertices.reserve(vertices.size() + 18);
                    normals.reserve(normals.size() + 18);
                    uvs.reserve(uvs.size() + 12);

                    // Vertices
                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z);           // Triangle 1
                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z);
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z);

                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z);           // Triangle 2
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z);
                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z);

                    // Normals (back face normal: 0.0f, 0.0f, -1.0f)
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(-1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(-1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(-1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(-1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(-1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(-1.0f);

                    // UVs
                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);

                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(0.0f); uvs.emplace_back(1.0f);
                }
                
                if (blocks[getBlockXYZ(x, y, z + 1)] == 0) {
                    // Add face to the front
                    vertices.reserve(vertices.size() + 18);
                    normals.reserve(normals.size() + 18);
                    uvs.reserve(uvs.size() + 12);

                    // Vertices
                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z + 1);       // Triangle 1
                    vertices.emplace_back(x + 1); vertices.emplace_back(y); vertices.emplace_back(z + 1);
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);

                    vertices.emplace_back(x); vertices.emplace_back(y); vertices.emplace_back(z + 1);       // Triangle 2
                    vertices.emplace_back(x + 1); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);
                    vertices.emplace_back(x); vertices.emplace_back(y + 1); vertices.emplace_back(z + 1);

                    // Normals (front face normal: 0.0f, 0.0f, 1.0f)
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(1.0f);
                    normals.emplace_back(0.0f); normals.emplace_back(0.0f); normals.emplace_back(1.0f);

                    // UVs
                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);

                    uvs.emplace_back(0.0f); uvs.emplace_back(0.0f);
                    uvs.emplace_back(1.0f); uvs.emplace_back(1.0f);
                    uvs.emplace_back(0.0f); uvs.emplace_back(1.0f);
                }
            }
        }
    }
    return BS::Mesh(BS::VertexBuffer(vertices, 3), {BS::VertexBuffer(normals, 3), BS::VertexBuffer(uvs, 2)}, BS::Mesh::TRIANGLES);
}