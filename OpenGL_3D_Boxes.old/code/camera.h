#pragma once

#include <vector>
#include <GLEW\glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>




const static GLfloat DEFYAW = -90.0f;
const static GLfloat DEFPITCH = 0.0f;
const static GLfloat DEFTRANSLATIONSPEED = 3.0f; //to move position
const static GLfloat DEFANGLESENSITIVITY = .25f; //to move yaw and pitch
const static GLfloat DEFZOOMSENSITIVITY = 1.f;
const static GLfloat DEFZOOMMIN = 30.0f;
const static GLfloat DEFZOOMMAX = 60.0f;




struct CameraProperties
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 up = glm::vec3(0.0f);
    glm::vec3 front = glm::vec3(0.0f);
    glm::vec3 right = glm::vec3(0.0f);


    GLfloat yaw = DEFYAW;
    GLfloat pitch = DEFPITCH;
    GLfloat zoom = DEFZOOMMIN;

    GLfloat translationSpeed = DEFTRANSLATIONSPEED;
    GLfloat angleSensitivity = DEFANGLESENSITIVITY;
    GLfloat zoomSensitivity = DEFANGLESENSITIVITY;

};


class Camera
{
public:
    enum Camera_Movement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera();
    ~Camera();
    bool Init();
    bool Init(glm::vec3 position,
              glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
              GLfloat yaw = DEFYAW,
              GLfloat pitch = DEFPITCH,
              GLfloat zoom = DEFZOOMMIN,
              GLfloat translationSpeed = DEFTRANSLATIONSPEED,
              GLfloat angleSensitivity = DEFANGLESENSITIVITY,
              GLfloat zoomSensititivity = DEFZOOMSENSITIVITY);

    glm::mat4 GetViewMatrix();
    void MoveCameraPosition(Camera_Movement direction, GLfloat dt);
    void ChangeViewAngle(GLfloat xOffset, GLfloat yOffset, GLboolean contrainPitch = true);
    void ChangeZoom(GLfloat yOffset);

    //temp
    friend class GraphicsController;
private:
    CameraProperties cameraProperties;
    void UpdateCameraVectors();
};