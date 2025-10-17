#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Vector.h"

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT };
enum CameraType { FPS, TPS };

// -- Default Values -- //
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1F;
const float FOV = 45.0f;
CameraType cameraType = CameraType::FPS;

class Camera {
   public:
    Vector3 Position;
    Vector3 Front;
    Vector3 Up;
    Vector3 Right;
    Vector3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // constructor with vectors
    Camera(Vector3 position = Vector3::Zero(), Vector3 worldUp = Vector3::Up(),
           float yaw = YAW, float pitch = PITCH)
        : Front(Vector3(0.0f, 0.0f, -1.0f)),
          MovementSpeed(SPEED),
          MouseSensitivity(SENSITIVITY),
          Fov(FOV) {
        Position = position;
        WorldUp = worldUp;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position.GLM(), Position.GLM() + Front.GLM(),
                           Up.GLM());
    }

    void ProcessKeyboardInputs(CameraMovement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD) Position += Front * velocity;
        if (direction == BACKWARD) Position -= Front * velocity;
        if (direction == LEFT) Position -= Right * velocity;
        if (direction == RIGHT) Position += Right * velocity;

        Position.y = 0;
        // -- FPS = Grounded
        // if (CameraType  CameraType::FPS){
    }

    void ProcessMouseMovement(float xoffset, float yoffset,
                              bool constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constrainPitch) {
            if (Pitch > 89.0f) Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset) {
        Fov -= (float)yoffset;
        if (Fov < 1.0f) Fov = 1.0f;
        if (Fov > 45.0f) Fov = 45.0f;
    }

   private:
    void updateCameraVectors() {
        Vector3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = front.Normalized();
        Right = Front.Cross(WorldUp).Normalized();
        Up = Right.Cross(Front).Normalized();
    }
};

#endif
