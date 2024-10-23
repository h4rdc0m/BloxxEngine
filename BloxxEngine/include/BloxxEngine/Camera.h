/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#pragma once
#include <glm/glm.hpp>

namespace BloxxEngine
{

class Camera
{

  public:
    enum class Movement
    {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down,
    };

    Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch);
    ~Camera() = default;

    /**
     * Returns the view matrix using Euler angles and the LookAt matrix.
     * @return the view matrix
     */
    [[nodiscard]] glm::mat4 GetViewMatrix() const;

    void Move(Movement direction, float deltaTime);
    void Rotate(float xOffset, float yOffset, bool constrainPitch = true);
    void Zoom(float yOffset);

    // Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float ZoomFactor;

  private:
    void UpdateCameraVectors();
};

} // namespace BloxxEngine
