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

class LightProbe : public Mapping
{
public:
    LightProbe() : Mapping("LightProbe") {}

    enum class MapType
    {
        Regular,
        Panoramic
    };
    MapType mapping_type = MapType::Regular;

    RTTR_ENABLE(Mapping)

}; // LightProbe

class SquareMap : public Mapping
{
public:
    SquareMap() : Mapping("SquareMap") {}

    RTTR_ENABLE(Mapping)

}; // SquareMap

class CylindricalMap : public Mapping
{
public:
    CylindricalMap() : Mapping("CylindricalMap") {}

    RTTR_ENABLE(Mapping)

}; // CylindricalMap

class HemisphericalMap : public Mapping
{
public:
    HemisphericalMap() : Mapping("HemisphericalMap") {}

    RTTR_ENABLE(Mapping)

}; // HemisphericalMap

}
}