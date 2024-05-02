//
// Created by Maxime Boulanger on 2023-11-17.
//

#include <thread>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include "GameEngine.h"

namespace engine {
    GameEngine::GameEngine(
            uint8_t fps,
            unsigned int screenWidth,
            unsigned int screenHeight) :
            _cube(Mesh::loadFromObjectFile("objects/space-ship.obj")),
            _screenWidth(screenWidth),
            _projectionMatrix(_computeProjectionMatrix(screenWidth, screenHeight)),
            _sleepTime(static_cast<long long>(1000. / fps)),
            _vCamera(Vec3DGraphic(0, 0, 0)),
            _window(sf::RenderWindow (sf::VideoMode(screenWidth, screenHeight), "3D Game Engine")),
            _lookDirection(Vec3DGraphic(0, 0, 1))
    {
        std::vector<Triangle3D> triangles = std::vector<Triangle3D>();
    }

    void GameEngine::startLoop()
    {
        // _window.setMouseCursorVisible(false);
        while (_window.isOpen())
        {
            try
            {
                auto startedAt = std::chrono::steady_clock::now();
                std::this_thread::sleep_for(std::chrono::milliseconds(_sleepTime));
                auto currentTime = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime- startedAt).count();
                _update(static_cast<float>(elapsedTime) / 100.f);
            }
            catch (GameEngineException &e)
            {
                break;
            }
        }
    }

    void GameEngine::_update(float elapsedTime)
    {

        _manageEvents(elapsedTime);
         _fTheta += 0.1F * elapsedTime;

        Matrix matRotZ(4, std::vector<float>({
            cosf(_fTheta), sinf(_fTheta), 0, 0,
            -sinf(_fTheta), cosf(_fTheta), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        }));

        Matrix matRotX(4, std::vector<float>({
             1, 0, 0, 0,
             0, cosf(_fTheta * 0.5f), sinf(_fTheta * 0.5f), 0,
             0, -sinf(_fTheta * 0.5f), cosf(_fTheta * 0.5f), 0,
             0, 0, 0, 1
        }));

        Matrix matRotY(4, std::vector<float>({
            cosf(_fYaw), 0, sinf(_fYaw), 0,
            0, 1 ,0, 0,
            -sinf(_fYaw), 0, cosf(_fYaw), 0,
            0, 0, 0, 1
        }));


        Matrix translationMatrix = Matrix::getIdentityMatrix(4);
        translationMatrix.set(3, 2, 16.f);
        Matrix worldMatrix = matRotZ * matRotX;
        worldMatrix = worldMatrix * translationMatrix;

//        Matrix worldMatrix = translationMatrix;

        Vec3DGraphic vUp = Vec3DGraphic(0, 1, 0);
        Vec3DGraphic vTarget = Vec3DGraphic(0, 0, 1);

        _lookDirection = Vec3DGraphic( vTarget * matRotY);

        vTarget = Vec3DGraphic(_vCamera + _lookDirection);


        Matrix cameraMatrix = computePointAtMatrix(_vCamera, vTarget, vUp);

        Matrix viewMatrix = computeLookAtMatrix(cameraMatrix);

        sf::VertexArray trianglesToDraw = sf::VertexArray(sf::Triangles, 3 * _cube.getTriangles().size());
        std::vector<Triangle3D> trianglesToRaster = std::vector<Triangle3D>();
        float rescaleFactor = 0.5f * static_cast<float>(_screenWidth);

        for(const auto & triangleMesh : _cube.getTriangles()) {
            Triangle3D triangle = triangleMesh * worldMatrix;

            Vec3DGraphic normal = triangle.getNormal();
            Vec3DGraphic p1AdjustedWithCamera = Vec3DGraphic(triangle.getP1() - _vCamera);
            if(normal.dot(p1AdjustedWithCamera) < 0.f){
                Vec3DGraphic lightDirection = Vec3DGraphic(0.f, 0.f, -1.f);
                lightDirection.normalize();
                Triangle3D triangleProjected = (triangle * viewMatrix * _projectionMatrix).translate(1.0f, 1.0f, 0.0f) * rescaleFactor;
                triangleProjected.setLight(lightDirection.dot(normal) * 255.f);
                trianglesToRaster.push_back(triangleProjected);
            }
        }

        std::sort(trianglesToRaster.begin(), trianglesToRaster.end(), [](const auto &triangle1, const auto &triangle2) {
            return triangle1.getZMean() < triangle2.getZMean();
        });


        for(int i = 0; i < trianglesToRaster.size(); ++i) {
            Triangle3D triangle = trianglesToRaster[i];
            const Vec3DGraphic& p1 = triangle.getP1();
            const Vec3DGraphic& p2 = triangle.getP2();
            const Vec3DGraphic& p3 = triangle.getP3();
            trianglesToDraw[i * 3].position = sf::Vector2f(p1.getX(), p1.getY());
            auto r = sf::Uint8(std::max(30.f, triangle.getLight()));
            auto g = sf::Uint8(std::max(30.f, triangle.getLight()));
            auto b = sf::Uint8(std::max(30.f, triangle.getLight()));
            sf::Color color = sf::Color(r, g, b);
            trianglesToDraw[i * 3].color = color;
            trianglesToDraw[i * 3 + 1].position = sf::Vector2f(p2.getX(), p2.getY());
            trianglesToDraw[i * 3 + 1].color =  color;
            trianglesToDraw[i * 3 + 2].position = sf::Vector2f(p3.getX(), p3.getY());
            trianglesToDraw[i * 3 + 2].color = color;
        }

        _window.clear();
        _window.draw(trianglesToDraw);
        _window.display();
    }

    void GameEngine::_manageEvents(float elapsedTime)
    {
        sf::Event event{};

        while (_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                _window.close();
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.scancode == sf::Keyboard::Scan::Down)
                {
                    _vCamera.setY(_vCamera.getY() - 8.f * elapsedTime);
                }
                else if(event.key.scancode == sf::Keyboard::Scan::Up)
                {
                    _vCamera.setY(_vCamera.getY() + 8.f * elapsedTime);
                }
                else if(event.key.scancode == sf::Keyboard::Scan::Right)
                {
                    _vCamera.setX(_vCamera.getX() - 8.f * elapsedTime);
                }
                else if(event.key.scancode == sf::Keyboard::Scan::Left)
                {
                    _vCamera.setX(_vCamera.getX() + 8.f * elapsedTime);
                }
                else if(event.key.scancode == sf::Keyboard::Scan::A)
                {
                    _fYaw -= 0.5f * elapsedTime;
                }
                else if(event.key.scancode == sf::Keyboard::Scan::D)
                {
                    _fYaw += 0.5f * elapsedTime;
                }
                else if(event.key.scancode == sf::Keyboard::Scan::W)
                {
                    Vec3DGraphic vForward = Vec3DGraphic(_lookDirection * 8.f * elapsedTime);
                    _vCamera = Vec3DGraphic(_vCamera + vForward);
                }
                else if(event.key.scancode == sf::Keyboard::Scan::S)
                {
                    Vec3DGraphic vForward = Vec3DGraphic(_lookDirection * 8.f * elapsedTime);
                    _vCamera = Vec3DGraphic(_vCamera - vForward);
                }
            }
            else if (event.type == sf::Event::MouseMoved)
            {

            }
        }
    }

    Matrix GameEngine::_computeProjectionMatrix(unsigned int width, unsigned int height) {

        const float PI = 3.14159f;
        float zNear = 0.1f;
        float zFar = 1000.0f;
        float fieldOfViewAngle = 270.f;
        float aspectRatio = static_cast<float>(height) / static_cast<float>(width);
        float fieldOfViewRadians = 1.0f / tanf(fieldOfViewAngle * 0.5f / 180.0f * static_cast<float>(M_PI));

        return Matrix::getProjectionMatrix(aspectRatio, fieldOfViewRadians, zFar, zNear);
    }

    Matrix GameEngine::computePointAtMatrix(const Vec3DGraphic &pos, const Vec3DGraphic &target, const Vec3DGraphic &up) {
        Vec3DGraphic newForward = Vec3DGraphic(target - pos);
        newForward.normalize();
        Vec3DGraphic a = Vec3DGraphic(newForward * up.dot(newForward));
        Vec3DGraphic newUp = Vec3DGraphic(up -a);
        newUp.normalize();

        Vec3DGraphic newRight = newUp.crossProduct(newForward);

        Matrix pointAt = Matrix(4, std::vector<float>({
            newRight.getX(), newRight.getY(), newRight.getZ(), 0.f,
            newUp.getX(), newUp.getY(), newUp.getZ(), 0.f,
            newForward.getX(), newForward.getY(), newForward.getZ(), 0.f,
            pos.getX(), pos.getY(), pos.getZ(), 1.f
        }));
        return pointAt;
    }

    Matrix GameEngine::computeLookAtMatrix(const Matrix &m) {
        return Matrix(4, std::vector<float>({
            m.at(0, 0), m.at(1, 0), m.at(2, 0), 0.f,
            m.at(0, 1), m.at(1, 1), m.at(2, 1), 0.f,
            m.at(0, 2), m.at(1, 2), m.at(2, 2), 0.f,
            -(m.at(3, 0) * m.at(0, 0) + m.at(3, 1) * m.at(0,1) + m.at(3, 2) * m.at(0, 2)),
            -(m.at(3, 0) * m.at(1, 0) + m.at(3, 1) * m.at(1,1) + m.at(3, 2) * m.at(1, 2)),
            -(m.at(3, 0) * m.at(2, 0) + m.at(3, 1) * m.at(2,1) + m.at(3, 2) * m.at(2, 2)),
            1.0f
        // TODO Finish
        }));
    }
} // engine