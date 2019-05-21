#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Camera : public Node
{
public:
    Camera(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_CAMERA, "Out", *this));

        Layout();
    }

    sm::vec3 pos = sm::vec3(0, 0, 500);
    sm::vec3 target;

    RTTR_ENABLE(Node)

}; // Camera

class Pinhole : public Camera
{
public:
    Pinhole() : Camera("Pinhole") {}

    float dis = 500.0f;
    float zoom = 1.0f;

    RTTR_ENABLE(Camera)

}; // Pinhole

class ThinLens : public Camera
{
public:
    ThinLens()
        : Camera("ThinLens")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_SAMPLER, "Sampler", *this));

        Layout();
    }

    float lens_radius = 1;	// lens radius
    float d = 500;			// view plane distance
    float f = 75;			// focal distance
    float zoom = 1;			// zoom factor

    RTTR_ENABLE(Camera)

}; // ThinLens

class FishEye : public Camera
{
public:
    FishEye() : Camera("FishEye") {}

    float fov = 180.0f;

    RTTR_ENABLE(Camera)

}; // FishEye

class Spherical : public Camera
{
public:
    Spherical() : Camera("Spherical") {}

    float hori_fov = 360.0f;
    float vert_fov = 180.0f;

    RTTR_ENABLE(Camera)

}; // Spherical

class Stereo : public Camera
{
public:
    Stereo()
        : Camera("Stereo")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_LEFT_CAM, PIN_CAMERA, "Left", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_RIGHT_CAM, PIN_CAMERA, "Right", *this));

        Layout();
    }

    enum class ViewingType
    {
        Parallel,
        Transverse
    };
    ViewingType viewing_type = ViewingType::Parallel;

    int pixel_gap = 0;

    float stereo_angle = 0.0f;

    enum InputID
    {
        ID_LEFT_CAM = 0,
        ID_RIGHT_CAM,
    };

    RTTR_ENABLE(Camera)

}; // Stereo

class Orthographic : public Camera
{
public:
    Orthographic() : Camera("Orthographic") {}

    float dis  = 0;
    float zoom = 1.0f;

    RTTR_ENABLE(Camera)

}; // Orthographic

}
}