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

class PlaneChecker : public Texture
{
public:
    PlaneChecker() : Texture("PlaneChecker") {}

    float outline_width = 0;		// width of the horizontal lines as a fraction of the checker width
    float size = 0;

    pt0::Color color1     = pt0::Color(1.0f, 1.0f, 1.0f);   // checker color 1
    pt0::Color color2     = pt0::Color(0.5f, 0.5f, 0.5f);   // checker color 2
    pt0::Color line_color = pt0::Color(0, 0, 0);            // the line color

    RTTR_ENABLE(Texture)

}; // PlaneChecker

class SphereChecker : public Texture
{
public:
    SphereChecker() : Texture("SphereChecker") {}

    int	     num_horizontal_checkers = 20;	           // number of checkers in the horizontal (azithum) direction
    int	     num_vertical_checkers   = 10;		       // number of checkers in the vertical (polar) direction
    float    horizontal_line_width   = 0.0f;		   // width of the horizontal lines as a fraction of the checker width
    float    vertical_line_width     = 0.0f;		   // width of the vertical lines as a fraction of the checker width

    pt0::Color color1     = pt0::Color(1.0f, 1.0f, 1.0f);   // checker color 1
    pt0::Color color2     = pt0::Color(0.5f, 0.5f, 0.5f);   // checker color 2
    pt0::Color line_color = pt0::Color(0, 0, 0);            // the line color

    RTTR_ENABLE(Texture)

}; // SphereChecker

class ConstantColor : public Texture
{
public:
    ConstantColor() : Texture("ConstantColor") {}

    pt0::Color color = pt0::Color(1.0f, 1.0f, 1.0f);

    RTTR_ENABLE(Texture)

}; // ConstantColor

class WrappedFBmTexture : public Texture
{
public:
    WrappedFBmTexture()
        : Texture("WrappedFBmTexture")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_NOISE, "Noise", *this));

        Layout();
    }

    float max_value = 1;
    float min_value = 0;

    float expansion_number = 0;

    pt0::Color color;

    RTTR_ENABLE(Texture)

}; // WrappedFBmTexture

class RampFBmTexture : public Texture
{
public:
    RampFBmTexture() : Texture("RampFBmTexture") {}

    std::string filepath;

    int   num_octaves = 0;
    float fbm_amount  = 0;

    RTTR_ENABLE(Texture)

}; // RampFBmTexture

class TextureInstance : public Texture
{
public:
    TextureInstance()
        : Texture("TextureInstance")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_TEXTURE, "Texture", *this));

        Layout();
    }

    sm::vec3 scale = sm::vec3(1, 1, 1);
    sm::vec3 rotate;
    sm::vec3 translate;

    RTTR_ENABLE(Texture)

}; // TextureInstance

class Wood : public Texture
{
public:
    Wood() : Texture("Wood") {}

    pt0::Color light = pt0::Color(1.0f, 1.0f, 1.0f);
    pt0::Color dark  = pt0::Color(0, 0, 0);

    RTTR_ENABLE(Texture)

}; // Wood

class TurbulenceTexture : public Texture
{
public:
    TurbulenceTexture()
        : Texture("TurbulenceTexture")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_NOISE, "Noise", *this));

        Layout();
    }

    pt0::Color color;
    float min_val = 0;
    float max_val = 1;

    RTTR_ENABLE(Texture)

}; // TurbulenceTexture

class WrappedTwoColors : public Texture
{
public:
    WrappedTwoColors()
        : Texture("WrappedTwoColors")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_NOISE, "Noise", *this));

        Layout();
    }

    float min_val = 0;
    float max_val = 1;

    float exp = 0;

    pt0::Color color1;
    pt0::Color color2;

    RTTR_ENABLE(Texture)

}; // WrappedTwoColors

class WrappedThreeColors : public Texture
{
public:
    WrappedThreeColors()
        : Texture("WrappedThreeColors")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_NOISE, "Noise", *this));

        Layout();
    }

    float min_val = 0;
    float max_val = 1;

    float exp = 0;

    pt0::Color color1;
    pt0::Color color2;
    pt0::Color color3;

    RTTR_ENABLE(Texture)

}; // WrappedThreeColors

class NestedNoisesTexture : public Texture
{
public:
    NestedNoisesTexture()
        : Texture("NestedNoisesTexture")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_NOISE,   PIN_NOISE,   "Noise",   *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_TEXTURE, PIN_TEXTURE, "Texture", *this));

        Layout();
    }

    float min_val = 0;
    float max_val = 1;

    float exp = 0;
    float wrap_factor = 0;

    pt0::Color color;

    enum InputID
    {
        ID_NOISE = 0,
        ID_TEXTURE,
    };

    RTTR_ENABLE(Texture)

}; // NestedNoisesTexture

class WrappedRamp : public Texture
{
public:
    WrappedRamp()
        : Texture("WrappedRamp")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_NOISE, "Noise", *this));

        Layout();
    }

    float min_val = 0;
    float max_val = 1;

    float exp = 0;

    pt0::Color color;

    float perturbation = 0;
    float wrap_number  = 0;
    int   hres         = 0;

    std::string filepath;

    RTTR_ENABLE(Texture)

}; // WrappedRamp

}
}
