
#include "camera.h"
#include <iostream>


Camera::Camera()
{
}

Camera::~Camera()
{
}

bool Camera::Init(glm::vec3 position,
                  glm::vec3 worldUp,
                  GLfloat yaw,
                  GLfloat pitch,
                  GLfloat zoom,
                  GLfloat translationSpeed,
                  GLfloat angleSensitvity,
                  GLfloat zoomSensivity)
{
    this->cameraProperties.position = position;
    this->cameraProperties.worldUp = worldUp;
    this->cameraProperties.yaw = yaw;
    this->cameraProperties.pitch = pitch;
    this->cameraProperties.zoom = zoom;
    this->cameraProperties.translationSpeed = DEFTRANSLATIONSPEED;
    this->cameraProperties.angleSensitivity = DEFANGLESENSITIVITY;
    this->cameraProperties.zoomSensitivity = DEFZOOMSENSITIVITY;

    this->UpdateCameraVectors();

    return true;
}

bool Camera::Init()
{
    this->UpdateCameraVectors();

    return true;
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->cameraProperties.position, this->cameraProperties.position + this->cameraProperties.front, this->cameraProperties.up);
}

void Camera::MoveCameraPosition(Camera_Movement direction, GLfloat dt)
{

    GLfloat velocity = this->cameraProperties.translationSpeed * dt;
    if (direction == FORWARD)
        this->cameraProperties.position += this->cameraProperties.front * velocity;
    if (direction == BACKWARD)
        this->cameraProperties.position -= this->cameraProperties.front * velocity;
    if (direction == LEFT)
        this->cameraProperties.position -= this->cameraProperties.right * velocity;
    if (direction == RIGHT)
        this->cameraProperties.position += this->cameraProperties.right * velocity;

    UpdateCameraVectors();
}

void Camera::ChangeViewAngle(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch)
{
    xOffset *= this->cameraProperties.angleSensitivity;
    yOffset *= this->cameraProperties.angleSensitivity;

    this->cameraProperties.yaw += xOffset;
    this->cameraProperties.pitch += yOffset;

    if (constrainPitch)
    {
        if (this->cameraProperties.pitch > 89.0f)
        {
            this->cameraProperties.pitch = 89.0f;
        }
        if (this->cameraProperties.pitch < -89.0f)
        {
            this->cameraProperties.pitch = -89.0f;
        }
    }
    UpdateCameraVectors();
}

void Camera::ChangeZoom(GLfloat yOffset)
{

    if (this->cameraProperties.zoom >= DEFZOOMMIN && this->cameraProperties.zoom <= DEFZOOMMAX)
    {
        this->cameraProperties.zoom += yOffset * this->cameraProperties.zoomSensitivity;
    }
    if (this->cameraProperties.zoom <= DEFZOOMMIN)
    {
        this->cameraProperties.zoom = DEFZOOMMIN;
    }
    if (this->cameraProperties.zoom >= DEFZOOMMAX)
    {
        this->cameraProperties.zoom = DEFZOOMMAX;
    }
    //std::cout << cameraProperties.zoom << std::endl;
}


void Camera::UpdateCameraVectors()
{
    glm::vec3 tempFront;
    tempFront.x = cos(glm::radians(this->cameraProperties.yaw)) * cos(glm::radians(this->cameraProperties.pitch));
    tempFront.y = sin(glm::radians(this->cameraProperties.pitch));
    tempFront.z = sin(glm::radians(this->cameraProperties.yaw)) * cos(glm::radians(this->cameraProperties.pitch));

    this->cameraProperties.front = glm::normalize(tempFront);

    this->cameraProperties.right = glm::normalize(glm::cross(this->cameraProperties.front, this->cameraProperties.worldUp));
    this->cameraProperties.up = glm::normalize(glm::cross(this->cameraProperties.right, this->cameraProperties.front));

}
