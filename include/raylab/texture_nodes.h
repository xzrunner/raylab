#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Texture : public Node
{
public:
    Texture(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Texture

class Checker3D : public Texture
{
public:
    Checker3D() : Texture("Checker3D") {}

    pt0::Color col0;
    pt0::Color col1;

    float size = 0;

    RTTR_ENABLE(Texture)

}; // Checker3D

}
}
