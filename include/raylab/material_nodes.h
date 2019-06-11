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
        AddPin(std::make_shared<bp::Pin>(true, ID_KA,      bp::PIN_FLOAT1, "ka",      *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_KD,      bp::PIN_FLOAT1, "kd",      *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_CD,      bp::PIN_FLOAT3, "cd",      *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_SAMPLER, PIN_SAMPLER,    "Sampler", *this));

        Layout();
    }

    float ka = 0;
    float kd = 0;
    pt0::Color cd;

    enum InputID
    {
        ID_KA = 0,
        ID_KD,
        ID_CD,
        ID_SAMPLER,
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
        AddPin(std::make_shared<bp::Pin>(true, 0, bp::PIN_FLOAT1, "ka", *this));
        AddPin(std::make_shared<bp::Pin>(true, 1, bp::PIN_FLOAT1, "kd", *this));
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

    float radiance_scale_factor = 1.0f;
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
    float exp = 1.0f;

    RTTR_ENABLE(Material)

}; // Phong

class Reflective : public Phong
{
public:
    Reflective() : Phong("Reflective") {}

    float      kr = 0.0f;
    pt0::Color cr = pt0::Color(1.0f, 1.0f, 1.0f);

    RTTR_ENABLE(Phong)

}; // Reflective

class SV_Emissive : public Material
{
public:
    SV_Emissive()
        : Material("SV_Emissive")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_TEXTURE, "tex", *this));

        Layout();
    }

    float radiance_scale_factor = 1.0f;
    pt0::Color color;

    RTTR_ENABLE(Material)

}; // SV_Emissive

class GlossyReflector : public Phong
{
public:
    GlossyReflector()
        : Phong("GlossyReflector")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_SAMPLER, "Sampler", *this));

        Layout();
    }

    float      kr  = 0.0f;
    pt0::Color cr  = pt0::Color(255, 255, 255);

    int        num_samples = 0;

    RTTR_ENABLE(Phong)

}; // GlossyReflector

class Transparent : public Phong
{
public:
    Transparent() : Phong("Transparent") {}

    float ior = 0.0f;
    float kr  = 0.0f;
    float kt  = 0.0f;

    RTTR_ENABLE(Phong)

}; // Transparent

class Dielectric : public Phong
{
public:
    Dielectric() : Phong("Dielectric") {}

    float eta_in  = 0.0f;
    float eta_out = 0.0f;
    pt0::Color cf_in;
    pt0::Color cf_out;

    RTTR_ENABLE(Phong)

}; // Dielectric

class SphereMaterials : public Material
{
public:
    SphereMaterials()
        : Material("SphereMaterials")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_CHECKER1_MATERIAL, PIN_MATERIAL, "checker1", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_CHECKER2_MATERIAL, PIN_MATERIAL, "checker2", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_LINE_MATERIAL,     PIN_MATERIAL, "line",     *this));

        Layout();
    }

    int	  num_horizontal_checkers = 10;	// number of checkers in the horizontal (azithum) direction
    int   num_vertical_checkers   = 20;	// number of checkers in the vertical (polar) direction
    float horizontal_line_width   = 0;	// width of the horizontal lines as a fraction of the checker width
    float vertical_line_width     = 0;	// width of the vertical lines as a fraction of the checker width
    pt0::Color color1     = pt0::Color(1.0f, 1.0f, 1.0f);	// checker color 1
    pt0::Color color2     = pt0::Color(0.5f, 0.5f, 0.5f);	// checker color 2
    pt0::Color line_color = pt0::Color(0, 0, 0);			// the line color

    enum InputID
    {
        ID_CHECKER1_MATERIAL = 0,
        ID_CHECKER2_MATERIAL,
        ID_LINE_MATERIAL,
    };

    RTTR_ENABLE(Material)

}; // SphereMaterials

class SV_Phong : public Material
{
public:
    SV_Phong()
        : Material("SV_Phong")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_CD_TEXTURE, PIN_TEXTURE, "cd", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_CS_TEXTURE, PIN_TEXTURE, "cs", *this));

        Layout();
    }

    float ka  = 0.0f;
    float kd  = 0.0f;
    float ks  = 0.0f;
    float exp = 1.0f;

    enum InputID
    {
        ID_CD_TEXTURE = 0,
        ID_CS_TEXTURE,
    };

    RTTR_ENABLE(Material)

}; // SV_Phong

class SV_Reflector : public Phong
{
public:
    SV_Reflector()
        : Phong("SV_Reflector")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_TEXTURE, "cr", *this));

        Layout();
    }

    float kr  = 0.0f;

    RTTR_ENABLE(Material)

}; // SV_Reflector

}
}