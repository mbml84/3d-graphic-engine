//
// Created by Maxime Boulanger on 2023-11-17.
//

#ifndef INC_3DGRAPHICSENGINE_GAMEENGINE_H
#define INC_3DGRAPHICSENGINE_GAMEENGINE_H

#include <ctime>
#include <cstdint>
#include <iostream>
#include "shapes/Mesh.h"

namespace engine {


class GameEngineException : public std::runtime_error {};

    class GameEngine {
    public:
        GameEngine(uint8_t fps, unsigned int screenWidth, unsigned int screenHeight);

    private:

        long long _sleepTime;

        unsigned int _screenWidth;

        sf::RenderWindow _window;

        Matrix _projectionMatrix;

        Mesh _cube;

        float _fTheta = 0.0f;

        float _fYaw = 0.0f;

        Vec3DGraphic _vCamera;

        Vec3DGraphic _lookDirection;

        void _update(float elapsedTime);

        void _manageEvents(float elapsedTime);

    public:
        void startLoop();

        static Matrix _computeProjectionMatrix(unsigned int width, unsigned int height);

        static Matrix computePointAtMatrix(const Vec3DGraphic &pos, const Vec3DGraphic &target, const Vec3DGraphic &up);

        static Matrix computeLookAtMatrix(const Matrix &m);
    };

} // engine

#endif //INC_3DGRAPHICSENGINE_GAMEENGINE_H
