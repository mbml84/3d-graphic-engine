//
// Created by Maxime Boulanger on 2023-11-17.
//

#ifndef INC_3DGRAPHICSENGINE_MATRIX_H
#define INC_3DGRAPHICSENGINE_MATRIX_H

#include <vector>

namespace engine {

    class Matrix {
    public:

        Matrix(size_t rows, const std::vector<float> &data);

        Matrix(const Matrix &m);

        [[nodiscard]] float at(size_t row, size_t col) const;

        void set(size_t row, size_t col, float val);

        [[nodiscard]] size_t rows() const;

        [[nodiscard]] size_t cols() const;

        Matrix getTransposition() const;

        void transpose();

        Matrix operator+(const Matrix &m) const;

        Matrix operator-(const Matrix &m) const;

        Matrix operator*(const Matrix &m) const;

        Matrix operator*(float scalar);

        void operator*=(float scalar);

        static Matrix getProjectionMatrix(float aspectRatio, float fieldOfView, float zFar, float zNear);

        static Matrix zeros(size_t rows, size_t cols);

        static Matrix ones(size_t rows, size_t cols);

        static Matrix fill(size_t rows, size_t cols, float value);

        static Matrix getIdentityMatrix(size_t size);


    protected:
        size_t _nbRows;
        std::vector<float> _data;

    };

    class Vec : public Matrix {
    public:
        explicit Vec(const std::vector<float> &data);

        explicit Vec(const Matrix &m);

    };

    class Vec3DGraphic: public Vec {

    public:
        Vec3DGraphic(float x, float y, float z, float w);

        Vec3DGraphic();

        Vec3DGraphic(float x, float y, float z);

        explicit Vec3DGraphic(const Matrix &m);

        float getX() const;

        float getY() const;

        float getZ() const;

        void setX(float x);

        void setY(float y);

        void setZ(float z);

        Vec3DGraphic multiplyByMatrix(const Matrix &m) const;

        Vec3DGraphic translate(float xShift, float yShift, float zShift);

        float getNorm() const;

        float dot(const Vec3DGraphic &v) const;

        void normalize();

        Vec3DGraphic crossProduct(const Vec3DGraphic &v) const;
    };

} // engine

#endif //INC_3DGRAPHICSENGINE_MATRIX_H
