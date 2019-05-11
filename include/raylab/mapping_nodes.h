#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

namespace raylab
{
namespace node
{

class Mapping : public Node
{
public:
    Mapping(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_MAPPING, "Out", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Mapping

class SphericalMap : public Mapping
{
public:
    SphericalMap() : Mapping("SphericalMap") {}

    RTTR_ENABLE(Mapping)

}; // SphericalMap

}
}