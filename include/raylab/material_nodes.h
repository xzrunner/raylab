#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

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
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_MATERIAL, "Out", *this));

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

class Texture;

class SV_Matte : public Material
{
public:
    SV_Matte()
        : Material("SV_Matte")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, bp::PIN_FLOAT, "ka", *this));
        AddPin(std::make_shared<bp::Pin>(true, 1, bp::PIN_FLOAT, "kd", *this));
        AddPin(std::make_shared<bp::Pin>(true, 2, PIN_TEXTURE, "cd", *this));

        Layout();
    }

    float ka = 0;
    float kd = 0;
    std::shared_ptr<Texture> cd = nullptr;

    enum InputID
    {
        ID_KA = 0,
        ID_KD,
        ID_CD,
    };

    RTTR_ENABLE(Material)

}; // SV_Matte

class Emissive : public Material
{
public:
    Emissive() : Material("Emissive") {}

    float radiance_scale_factor = 0.0f;
    pt0::Color color;

    RTTR_ENABLE(Material)

}; // Emissive

class Phong : public Material
{
public:
    Phong(const std::string& title = "Phong") : Material(title) {}

    float ka = 0.0f;
    float kd = 0.0f;
    float ks = 0.0f;
    pt0::Color cd;
    pt0::Color cs = pt0::Color(1.0f, 1.0f, 1.0f);
    float exp = 0.0f;

    RTTR_ENABLE(Material)

}; // Phong

class Reflective : public Phong
{
public:
    Reflective() : Phong("Reflective") {}

    float      kr = 0.0f;
    pt0::Color cr;

    RTTR_ENABLE(Phong)

}; // Reflective

}
}