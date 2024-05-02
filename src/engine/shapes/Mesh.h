//
// Created by Maxime Boulanger on 2023-11-17.
//

#ifndef INC_3DGRAPHICSENGINE_MESH_H
#define INC_3DGRAPHICSENGINE_MESH_H

#include <vector>
#include "Triangle3D.h"

namespace engine {

    class Mesh {
    public:
        explicit Mesh(const std::vector<Triangle3D> &triangles);

        Mesh();

        [[nodiscard]] const std::vector<Triangle3D> &getTriangles() const;

        void setTriangles(const std::vector<Triangle3D> &triangles);

        static Mesh loadFromObjectFile(const std::string &filename);

    private:
        std::vector<Triangle3D> _triangles;
    };

} // engine

#endif //INC_3DGRAPHICSENGINE_MESH_H
