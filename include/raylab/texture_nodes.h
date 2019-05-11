#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

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
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_TEXTURE, "Out", *this));

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

class ImageTexture : public Texture
{
public:
    ImageTexture()
        : Texture("ImageTexture")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_MAPPING, "Mapping", *this));

        Layout();
    }

    std::string filepath;

    RTTR_ENABLE(Texture)

}; // ImageTexture

}
}
