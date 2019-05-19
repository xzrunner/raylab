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

class ConeChecker : public Texture
{
public:
    ConeChecker() : Texture("ConeChecker") {}

    int	     num_horizontal_checkers = 20;	           // number of checkers in the horizontal (azithum) direction
    int	     num_vertical_checkers   = 10;		       // number of checkers in the vertical (polar) direction
    float    horizontal_line_width   = 0.0f;		   // width of the horizontal lines as a fraction of the checker width
    float    vertical_line_width     = 0.0f;		   // width of the vertical lines as a fraction of the checker width

    pt0::Color color1     = pt0::Color(1.0f, 1.0f, 1.0f);   // checker color 1
    pt0::Color color2     = pt0::Color(0.5f, 0.5f, 0.5f);   // checker color 2
    pt0::Color line_color = pt0::Color(0, 0, 0);            // the line color

    RTTR_ENABLE(Texture)

}; // ConeChecker

}
}
