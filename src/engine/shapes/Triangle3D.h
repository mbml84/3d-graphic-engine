//
// Created by Maxime Boulanger on 2023-11-17.
//

#ifndef INC_3DGRAPHICSENGINE_TRIANGLE3D_H
#define INC_3DGRAPHICSENGINE_TRIANGLE3D_H

#include "Matrix.h"

namespace engine {

    class Triangle3D {
    public:
        Triangle3D(Vec3DGraphic p1, Vec3DGraphic p2, Vec3DGraphic p3);

        [[nodiscard]] const Vec3DGraphic &getP1() const;

        void setP1(const Vec3DGraphic &p1);

        [[nodiscard]] const Vec3DGraphic &getP2() const;

        void setP2(const Vec3DGraphic &p2);

        [[nodiscard]] const Vec3DGraphic &getP3() const;

        void setP3(const Vec3DGraphic &p3);

        [[nodiscard]] Vec3DGraphic getNormal() const;

        [[nodiscard]] bool isVisible() const;

        Triangle3D operator*(const Matrix &projectionMatrix) const;

        Triangle3D translate(float xShift, float yShift, float zShift);

        Triangle3D operator*(float factor);

        float getZMean() const;

        float getLight() const;

        void setLight(float light);

    private:
        Vec3DGraphic _p1;
        Vec3DGraphic _p2;
        Vec3DGraphic _p3;

        float _light;
    };

} // engine

#endif //INC_3DGRAPHICSENGINE_TRIANGLE3D_H
