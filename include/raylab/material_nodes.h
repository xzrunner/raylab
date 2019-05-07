#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Material : public Node
{
public:
    Material(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Material

class Matte : public Material
{
public:
    Matte()
        : Material("Matte")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, bp::PIN_FLOAT, "ka", *this));
        AddPin(std::make_shared<bp::Pin>(true, 1, bp::PIN_FLOAT, "kd", *this));

        Layout();
    }

    float ka = 0;
    float kd = 0;
    pt0::Color cd;

    enum InputID
    {
        ID_KA = 0,
        ID_KD,
    };

    RTTR_ENABLE(Material)

}; // Matte

}
}