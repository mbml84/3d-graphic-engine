//
// Created by Maxime Boulanger on 2023-11-17.
//

#include "Triangle3D.h"

#include <utility>

namespace engine {

    Triangle3D::Triangle3D(Vec3DGraphic p1, Vec3DGraphic p2, Vec3DGraphic p3) : _p1(std::move(p1)), _p2(std::move(p2)), _p3(std::move(p3)), _light(0.f) {}

    const Vec3DGraphic &Triangle3D::getP1() const {
        return _p1;
    }

    void Triangle3D::setP1(const Vec3DGraphic &p1) {
        _p1 = p1;
    }

    const Vec3DGraphic &Triangle3D::getP2() const {
        return _p2;
    }

    void Triangle3D::setP2(const Vec3DGraphic &p2) {
        _p2 = p2;
    }

    const Vec3DGraphic &Triangle3D::getP3() const {
        return _p3;
    }

    void Triangle3D::setP3(const Vec3DGraphic &p3) {
        _p3 = p3;
    }



    Triangle3D Triangle3D::operator*(const Matrix &matrix) const {
        Vec3DGraphic p1Proj = _p1.multiplyByMatrix(matrix);
        Vec3DGraphic p2Proj = _p2.multiplyByMatrix(matrix);
        Vec3DGraphic p3Proj = _p3.multiplyByMatrix(matrix);
        return {p1Proj, p2Proj, p3Proj};
    }

    Triangle3D Triangle3D::translate(float xShift, float yShift, float zShift) {
        return {_p1.translate(xShift, yShift, zShift), _p2.translate(xShift, yShift, zShift), _p3.translate(xShift, yShift, zShift)};
    }

    Triangle3D Triangle3D::operator*(float factor) {
        return {Vec3DGraphic(_p1 * factor), Vec3DGraphic(_p2 * factor), Vec3DGraphic(_p3 * factor)};
    }

    Vec3DGraphic Triangle3D::getNormal() const {
        Vec3DGraphic A = Vec3DGraphic(_p2 - _p1);
        Vec3DGraphic B = Vec3DGraphic(_p3 - _p1);
        Vec3DGraphic normal = A.crossProduct(B);
        normal.normalize();
        return normal;
    }

    bool Triangle3D::isVisible() const {
        Vec3DGraphic A = Vec3DGraphic(_p2 - _p1);
        Vec3DGraphic B = Vec3DGraphic(_p3 - _p1);
        return A.getX() * B.getY() - A.getY() * B.getX() < 0;
    }

    float Triangle3D::getZMean() const {
        return (_p1.getZ() + _p2.getZ() + _p3.getZ()) / 3.f;
    }

    float Triangle3D::getLight() const {
        return _light;
    }

    void Triangle3D::setLight(float light) {
        _light = light;
    }
} // engine