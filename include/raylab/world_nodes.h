#pragma once

#include "raylab/Node.h"

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
        AddPin(std::make_shared<bp::Pin>(true, 0, bp::PIN_ANY_VAR, "Light",  *this));
        AddPin(std::make_shared<bp::Pin>(true, 1, bp::PIN_ANY_VAR, "Tracer", *this));
        AddPin(std::make_shared<bp::Pin>(true, 2, bp::PIN_ANY_VAR, "Camera", *this));
        AddPin(std::make_shared<bp::Pin>(true, 3, bp::PIN_ANY_VAR, "Object", *this));
        AddPin(std::make_shared<bp::Pin>(true, 4, bp::PIN_ANY_VAR, "ViewPlane", *this));
        AddPin(std::make_shared<bp::Pin>(true, 5, bp::PIN_ANY_VAR, "Ambient", *this));

        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

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
        AddPin(std::make_shared<bp::Pin>(true, 0, bp::PIN_ANY_VAR, "In", *this));

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
        AddPin(std::make_shared<bp::Pin>(true, 0, bp::PIN_ANY_VAR, "Sampler", *this));
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

        Layout();
    }

    size_t width = 0, height = 0;

    float pixel_size = 1.0f;

    size_t num_samples = 0;

    RTTR_ENABLE(Node)

}; // ViewPlane

}
}