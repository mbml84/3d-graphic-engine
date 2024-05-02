//
// Created by Maxime Boulanger on 2023-11-17.
//

#include "Mesh.h"
#include <fstream>
#include <strstream>

namespace engine {
    Mesh::Mesh(const std::vector<Triangle3D> &triangles) : _triangles(triangles) {}

    Mesh::Mesh() : _triangles(std::vector<Triangle3D>()){}

    const std::vector<Triangle3D> &Mesh::getTriangles() const {
        return _triangles;
    }

    void Mesh::setTriangles(const std::vector<Triangle3D> &triangles) {
        _triangles = triangles;
    }

    Mesh Mesh::loadFromObjectFile(const std::string &filename) {
        std::ifstream f(filename);
        if (!f.is_open()) {
            throw std::runtime_error("Can't open " + filename);
        }
        std::vector<Vec3DGraphic> vertices;
        std::vector<Triangle3D> triangles = std::vector<Triangle3D>();

        while(!f.eof()) {
            char line[128];
            f.getline(line, 128);

            std::strstream s;
            s << line;

            char junk;

            if (line[0] == 'v')
            {
                float x, y, z;
                s >> junk >> x >> y >> z;
                vertices.emplace_back(x, y, z);
            }

            if (line[0] == 'f')
            {
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                triangles.emplace_back( vertices[f[0] - 1], vertices[f[1] - 1], vertices[f[2] - 1]);
            }
        }

        return Mesh(triangles);
    }


} // engine