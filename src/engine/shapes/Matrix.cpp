//
// Created by Maxime Boulanger on 2023-11-17.
//

#include "Matrix.h"
#include <iostream>

namespace engine {
    Matrix::Matrix(size_t rows, const std::vector<float> &data) : _nbRows(rows), _data(data) {
        if(data.size() % rows != 0) {
            throw std::runtime_error("Matrix rows must have the same amount of data");
        }
    }

    Matrix::Matrix(const Matrix &m) : _nbRows(m._nbRows), _data(std::vector<float>(m._data)) {}

    float Matrix::at(size_t row, size_t col) const {
        return _data.at(row * cols() + col);
    }

    size_t Matrix::rows() const {
        return _nbRows;
    }

    size_t Matrix::cols() const {
        return static_cast<size_t>(_data.size() / _nbRows);
    }

    void Matrix::set(size_t row, size_t col, float val) {
        _data[row * cols() + col] = val;
    }

    Matrix Matrix::getProjectionMatrix(float aspectRatio, float fieldOfView, float zFar, float zNear) {

        float q = zFar / (zFar - zNear);
        std::vector<float> data = std::vector<float>({
            aspectRatio * fieldOfView, 0, 0, 0,
            0, fieldOfView, 0, 0,
            0, 0, q, 1,
            0, 0, q*zNear, 0
        });
        return {4, data };
    }

    Matrix Matrix::getTransposition() const {
        Matrix t = Matrix(*this);
        t._nbRows = cols();
        return t;
    }

    void Matrix::transpose() {
        _nbRows = cols();
    }

    Matrix Matrix::operator+(const Matrix &m) const{
        if(cols() != m.cols() || _nbRows != m._nbRows) {
            throw std::runtime_error("Size don't match");
        }
        std::vector<float> newData = std::vector<float>();

        for(size_t i = 0; i < _data.size(); ++i) {
            newData.push_back(_data[i] + m._data[i]);
        }

        return {_nbRows, newData};
    }

    Matrix Matrix::operator-(const Matrix &m) const {
        if(cols() != m.cols() || _nbRows != m._nbRows) {
            throw std::runtime_error("Size don't match");
        }
        std::vector<float> newData = std::vector<float>();

        for(size_t i = 0; i < _data.size(); ++i) {
            newData.push_back(_data[i] - m._data[i]);
        }

        return {_nbRows, newData};
    }

    Matrix Matrix::operator*(const Matrix &m) const {
        if(cols() != m._nbRows) {
            throw std::runtime_error("Size don't match");
        }
        std::vector<float> newData = std::vector<float>();

        for(size_t i = 0; i < rows(); ++i) {
            for(size_t j = 0; j < m.cols(); ++j) {
                float newVal = 0;
                for(size_t k = 0; k < cols(); ++k) {
                    newVal += at(i, k) * m.at(k, j);
                }
                newData.push_back(newVal);
            }
        }
        return {rows(), newData};
    }

    Matrix Matrix::zeros(size_t rows, size_t cols) {
        return fill(rows, cols, 0);
    }

    Matrix Matrix::ones(size_t rows, size_t cols) {
        return fill(rows, cols, 1);
    }

    Matrix Matrix::fill(size_t rows, size_t cols, float value) {
        std::vector<float> data = std::vector<float>();

        for(int i = 0; i < rows * cols; ++i) {
            data.push_back(value);
        }
        return {rows, data};
    }

    Matrix Matrix::operator*(float scalar) {
        std::vector<float> newData = std::vector<float>();

        std::transform(_data.begin(), _data.end(), std::back_inserter(newData), [&scalar](float x) {
            return x * scalar;
        });

        return {_nbRows, newData};
    }

    void Matrix::operator*=(float scalar) {
        for(auto &x: _data) {
            x *= scalar;
        }
    }

    Matrix Matrix::getIdentityMatrix(size_t size) {
        std::vector<float> data = std::vector<float>();
        for(size_t i = 0; i < size; ++i) {
            for(size_t j = 0; j < size; ++j) {
                data.push_back((i == j)? 1.f : 0.f);
            }
        }

        return { size, data };
    }

    Vec::Vec(const std::vector<float> &data) : Matrix(1, data) {}

    Vec::Vec(const Matrix &m) : Matrix(m) {}

    Vec3DGraphic::Vec3DGraphic(const Matrix &m) : Vec(m) {}

    Vec3DGraphic::Vec3DGraphic(float x, float y, float z, float w) : Vec(std::vector<float>({x, y, z, w})) {}

    Vec3DGraphic::Vec3DGraphic(float x, float y, float z) : Vec(std::vector<float>({x, y, z, 1})){}

    Vec3DGraphic::Vec3DGraphic() : Vec(std::vector<float>({0, 0, 0, 1})){}

    Vec3DGraphic Vec3DGraphic::multiplyByMatrix(const Matrix &m) const {

        Matrix vec = Matrix::operator*(m);
        float w = vec.at(0, 3);
        if(w != 0.0f) {
            float wInv = 1 / w;
            vec *= wInv;
        }
        return Vec3DGraphic(vec);
    }

    float Vec3DGraphic::getX() const {
        return at(0, 0);
    }

    float Vec3DGraphic::getY() const {
        return at(0, 1);
    }

    float Vec3DGraphic::getZ() const {
        return at(0, 2);
    }

    Vec3DGraphic Vec3DGraphic::translate(float xShift, float yShift, float zShift) {
        return { getX() + xShift, getY() + yShift, getZ() + zShift };
    }

    float Vec3DGraphic::getNorm() const {
        return sqrtf(powf(getX(), 2) + powf(getY(), 2) + powf(getZ(), 2));
    }

    void Vec3DGraphic::normalize() {
        float norm = getNorm();
        for(auto &v : _data) {
            v /= norm;
        }
    }

    float Vec3DGraphic::dot(const Vec3DGraphic &v) const {
        return getX() * v.getX() + getY() * v.getY() + getZ() * v.getZ();
    }

    void Vec3DGraphic::setX(float x) {
        set(0, 0, x);
    }

    void Vec3DGraphic::setY(float y) {
        set(0, 1, y);
    }

    void Vec3DGraphic::setZ(float z) {
        set(0, 2, z);
    }

    Vec3DGraphic Vec3DGraphic::crossProduct(const Vec3DGraphic &v) const {
        return {getY() * v.getZ() - getZ() * v.getY(), getZ() * v.getX() - getX() * v.getZ(), getX() * v.getY() - getY() * v.getX()};
    }


} // engine