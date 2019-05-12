#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class World : public Node
{
public:
    World()
        : Node("World")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_LIGHT,      "Light",  *this));
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_TRACER,     "Tracer", *this));
        AddPin(std::make_shared<bp::Pin>(true, 2, PIN_CAMERA,     "Camera", *this));
        AddPin(std::make_shared<bp::Pin>(true, 3, PIN_OBJECT,     "Object", *this));
        AddPin(std::make_shared<bp::Pin>(true, 4, PIN_VIEW_PLANE, "ViewPlane", *this));
        AddPin(std::make_shared<bp::Pin>(true, 5, PIN_LIGHT,      "Ambient", *this));

        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_WORLD, "Out", *this));

        Layout();
    }

    enum InputID
    {
        ID_LIGHT = 0,
        ID_TRACER,
        ID_CAMERA,
        ID_OBJECT,
        ID_VIEW_PLANE,
        ID_AMBIENT,
    };

    RTTR_ENABLE(Node)

}; // World

class Output : public Node
{
public:
    Output()
        : Node("Output")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_WORLD, "In", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Output

class ViewPlane : public Node
{
public:
    ViewPlane()
        : Node("ViewPlane")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_SAMPLER,     "Sampler",     *this));
        AddPin(std::make_shared<bp::Pin>(true, 1, bp::PIN_INTEGER, "Num Samples", *this));

        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_VIEW_PLANE, "Out", *this));

        Layout();
    }

    int width = 0, height = 0;

    float pixel_size = 1.0f;

    int num_samples = 0;

    float max_depth = 0.0f;

    enum InputID
    {
        ID_SAMPLER = 0,
        ID_NUM_SAMPLES,
    };

    RTTR_ENABLE(Node)

}; // ViewPlane

}
}