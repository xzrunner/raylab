#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

#include <blueprint/Pin.h>

#include <sm_const.h>

namespace raylab
{
namespace node
{

class GeoPrimitive : public Node
{
public:
    GeoPrimitive(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(true,  0, PIN_MATERIAL, "Material", *this));
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_PRIMITIVE,   "Out", *this));

        Layout();
    }

    bool shadows = true;

    RTTR_ENABLE(Node)

}; // GeoPrimitive

class Compound : public GeoPrimitive
{
public:
    Compound()
        : GeoPrimitive("Compound")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_PRIMITIVE, "Children", *this));

        Layout();
    }

    enum InputID
    {
        ID_CHILDREN = 1,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // Compound

class Box : public GeoPrimitive
{
public:
    Box() : GeoPrimitive("Box") {}

    sm::vec3 min;
    sm::vec3 max;

    RTTR_ENABLE(GeoPrimitive)

}; // Box

class Sphere : public GeoPrimitive
{
public:
    Sphere()
        : GeoPrimitive("Sphere")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_SAMPLER, PIN_SAMPLER, "Sampler", *this));

        Layout();
    }

    sm::vec3 center;
    float radius = 1.0f;

    enum InputID
    {
        ID_SAMPLER = ID_MAX_BASE_INPUT_ID,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // Sphere

class Plane : public GeoPrimitive
{
public:
    Plane() : GeoPrimitive("Plane") {}

    sm::vec3 pos;
    sm::vec3 normal = sm::vec3(0, 1, 0);

    RTTR_ENABLE(GeoPrimitive)

}; // Plane

class Rectangle : public GeoPrimitive
{
public:
    Rectangle()
        : GeoPrimitive("Rectangle")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_SAMPLER,    "Sampler", *this));
        AddPin(std::make_shared<bp::Pin>(true, 2, bp::PIN_FLOAT3, "P0",      *this));
        Layout();
    }

    sm::vec3 p0     = sm::vec3(-1, 0, -1);
    sm::vec3 a      = sm::vec3(0, 0, 2);
    sm::vec3 b      = sm::vec3(2, 0, 0);
    sm::vec3 normal = sm::vec3(0, 0, 0);

    enum InputID
    {
        ID_SAMPLER = 1,
        ID_P0,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // Rectangle

class Triangle : public GeoPrimitive
{
public:
    Triangle() : GeoPrimitive("Triangle") {}

    sm::vec3 v0, v1, v2;
    sm::vec3 normal = sm::vec3(0, 1, 0);

    RTTR_ENABLE(GeoPrimitive)

}; // Triangle

class WireframeBox : public GeoPrimitive
{
public:
    WireframeBox() : GeoPrimitive("WireframeBox") {}

    sm::vec3 p0, p1;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // WireframeBox

class GeoInstance : public GeoPrimitive
{
public:
    GeoInstance() : GeoPrimitive("GeoInstance")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_PRIMITIVE, "Primitive", *this));
        Layout();
    }

    //enum class OpType
    //{
    //    Scale,
    //    Rotate,
    //    Translate
    //};

    //struct OpSRT
    //{
    //    OpType   type;
    //    sm::vec3 val;
    //};

//    std::vector<OpSRT> ops;

    sm::vec3 scale = sm::vec3(1, 1, 1);
    sm::vec3 rotate;
    sm::vec3 translate;

    bool transform_tex = true;

    enum InputID
    {
        ID_OBJECT = 1,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // GeoInstance

class Disk : public GeoPrimitive
{
public:
    Disk()
        : GeoPrimitive("Disk")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_SAMPLER,  "Sampler",  *this));

        Layout();
    }

    enum InputID
    {
        ID_SAMPLER = 1,
    };

    sm::vec3 center;
    sm::vec3 normal = sm::vec3(0, 1, 0);
    float    radius = 1.0;

    RTTR_ENABLE(GeoPrimitive)

}; // Disk

class SolidCylinder : public GeoPrimitive
{
public:
    SolidCylinder()
        : GeoPrimitive("SolidCylinder")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_MATERIAL, "Bottom Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, 2, PIN_MATERIAL, "Top Material",    *this));
        AddPin(std::make_shared<bp::Pin>(true, 3, PIN_MATERIAL, "Wall Material",   *this));

        Layout();
    }

    float bottom = 0;
    float top    = 0;
    float radius = 0;

    enum InputID
    {
        ID_BOTTOM_MATERIAL = 1,
        ID_TOP_MATERIAL,
        ID_WALL_MATERIAL,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // SolidCylinder

class ConvexPartCylinder : public GeoPrimitive
{
public:
    ConvexPartCylinder() : GeoPrimitive("ConvexPartCylinder") {}

    float bottom    = 0;
    float top       = 1;
    float radius    = 1;
    float polar_min = 0;
    float polar_max = 180;

    RTTR_ENABLE(GeoPrimitive)

}; // ConvexPartCylinder

class OpenPartCylinder : public GeoPrimitive
{
public:
    OpenPartCylinder() : GeoPrimitive("OpenPartCylinder") {}

    float bottom    = 0;
    float top       = 1;
    float radius    = 1;
    float polar_min = 0;
    float polar_max = 180;

    RTTR_ENABLE(GeoPrimitive)

}; // OpenPartCylinder

class Grid : public GeoPrimitive
{
public:
    Grid()
        : GeoPrimitive("Grid")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_PRIMITIVE, "Children", *this));

        Layout();
    }

    std::string filename;

    // Tessellate Sphere
    int hori_steps = 0;
    int vert_steps = 0;

    enum class TriangleType
    {
        Flat,
        Smooth
    };
    TriangleType triangle_type = TriangleType::Flat;

    enum InputID
    {
        ID_CHILDREN = 1,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // Grid

class ConcaveSphere : public GeoPrimitive
{
public:
    ConcaveSphere() : GeoPrimitive("ConcaveSphere") {}

    sm::vec3 center;
    float    radius = 1.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // ConcaveSphere

class Torus : public GeoPrimitive
{
public:
    Torus() : GeoPrimitive("Torus") {}

    float swept_radius = 2.0f;
    float tube_radius  = 0.5f;

    RTTR_ENABLE(GeoPrimitive)

}; // Torus

class OpenCone : public GeoPrimitive
{
public:
    OpenCone() : GeoPrimitive("OpenCone") {}

    float height = 2.0f;
    float radius = 1.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // OpenCone

class ConvexPartSphere : public GeoPrimitive
{
public:
    ConvexPartSphere() : GeoPrimitive("ConvexPartSphere") {}

    sm::vec3 center;
    float radius      = 1.0f;
    float azimuth_min = 0;
    float azimuth_max = 360;
    float polar_min   = 0;
    float polar_max   = 180;

    RTTR_ENABLE(GeoPrimitive)

}; // ConvexPartSphere

class ConcavePartSphere : public GeoPrimitive
{
public:
    ConcavePartSphere() : GeoPrimitive("ConcavePartSphere") {}

    sm::vec3 center;
    float radius      = 1.0f;
    float azimuth_min = 0;
    float azimuth_max = 360;
    float polar_min   = 0;
    float polar_max   = 180;

    RTTR_ENABLE(GeoPrimitive)

}; // ConcavePartSphere

class FlatRimmedBowl : public GeoPrimitive
{
public:
    FlatRimmedBowl() : GeoPrimitive("FlatRimmedBowl") {}

    float inner_radius = 0.9f;		// radius of the inside glass surface
    float outer_radius = 1.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // FlatRimmedBowl

class RoundRimmedBowl : public GeoPrimitive
{
public:
    RoundRimmedBowl() : GeoPrimitive("RoundRimmedBowl") {}

    float inner_radius   = 0.9f;		// radius of the inside glass surface
    float outer_radius   = 1.0f;
    float openning_angle = 180;

    RTTR_ENABLE(GeoPrimitive)

}; // RoundRimmedBowl

class ThickRing : public GeoPrimitive
{
public:
    ThickRing()
        : GeoPrimitive("ThickRing")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_MATERIAL, "Bottom Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, 2, PIN_MATERIAL, "Top Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, 3, PIN_MATERIAL, "Inner Wall Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, 4, PIN_MATERIAL, "Outer Wall Material", *this));

        Layout();
    }

    float bottom       = 0;
    float top          = 1;
    float inner_radius = 0.9f;
    float outer_radius = 1.0f;

    enum InputID
    {
        ID_BOTTOM_MATERIAL = 1,
        ID_TOP_MATERIAL,
        ID_INNER_WALL_MATERIAL,
        ID_OUTER_WALL_MATERIAL,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // ThickRing

class BeveledCylinder : public GeoPrimitive
{
public:
    BeveledCylinder() : GeoPrimitive("BeveledCylinder") {}

    float bottom       = 0.0f;
    float top          = 0.0f;
    float radius       = 0.0f;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // BeveledCylinder

class BeveledRing : public GeoPrimitive
{
public:
    BeveledRing() : GeoPrimitive("BeveledRing") {}

    float bottom       = 0.0f;
    float top          = 0.0f;
    float inner_radius = 0.0f;
    float outer_radius = 0.0f;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // BeveledRing

class BeveledBox : public GeoPrimitive
{
public:
    BeveledBox() : GeoPrimitive("BeveledBox") {}

    sm::vec3 bottom;
    sm::vec3 top;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // BeveledBox

class BeveledWedge : public GeoPrimitive
{
public:
    BeveledWedge() : GeoPrimitive("BeveledWedge") {}

    float min_y        = 0;
    float max_y        = 0;
    float inner_radius = 0;
    float outer_radius = 0;
    float bevel_radius = 0;
    float min_azimuth  = 0;
    float max_azimuth  = 0;

    RTTR_ENABLE(GeoPrimitive)

}; // BeveledWedge

class SolidCone : public GeoPrimitive
{
public:
    SolidCone() : GeoPrimitive("SolidCone") {}

    float radius = 1;
    float height = 2;

    RTTR_ENABLE(GeoPrimitive)

}; // SolidCone

class OpenCylinder : public GeoPrimitive
{
public:
    OpenCylinder() : GeoPrimitive("OpenCylinder") {}

    float bottom = -1;
    float top    = 1;
    float radius = 1;

    RTTR_ENABLE(GeoPrimitive)

}; // OpenCylinder

class ConcavePartCylinder : public GeoPrimitive
{
public:
    ConcavePartCylinder() : GeoPrimitive("ConcavePartCylinder") {}

    float bottom     = -1.0;       // bottom y value
    float top        = 1.0;        // top y value
    float radius     = 1.0;        // radius
    float inv_radius = 1.0;        // one over the radius
    float polar_min  = 0.0;
    float polar_max  = SM_TWO_PI;

    RTTR_ENABLE(GeoPrimitive)

}; // ConcavePartCylinder

class FishBowl : public GeoPrimitive
{
public:
    FishBowl()
        : GeoPrimitive("FishBowl")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_GLASS_AIR_MATERIAL,   PIN_MATERIAL, "Glass Air Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_WATER_AIR_MATERIAL,   PIN_MATERIAL, "Water Air Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_WATER_GLASS_MATERIAL, PIN_MATERIAL, "Water Glass Material", *this));

        Layout();
    }

    float inner_radius    = 1.0f;	// radius of the inside glass surface
    float glass_thickness = 0.1f;
    float water_depth     = 1.25f; 	// measured from the bottom of the water-glass boundary
    float meniscus_radius = 0.05f;
    float opening_angle   = 90;		// specifies how wide the opening is at the top (alpha in Figure 28.40(a))

    enum InputID
    {
        ID_GLASS_AIR_MATERIAL   = 1,
        ID_WATER_AIR_MATERIAL   = 2,
        ID_WATER_GLASS_MATERIAL = 3,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // FishBowl

class GlassOfWater : public GeoPrimitive
{
public:
    GlassOfWater()
        : GeoPrimitive("GlassOfWater")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_GLASS_AIR_MATERIAL,   PIN_MATERIAL, "Glass Air Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_WATER_AIR_MATERIAL,   PIN_MATERIAL, "Water Air Material", *this));
        AddPin(std::make_shared<bp::Pin>(true, ID_WATER_GLASS_MATERIAL, PIN_MATERIAL, "Water Glass Material", *this));

        Layout();
    }

    float height          = 2.0f;   // total height
    float inner_radius    = 0.9f;	// inner radius of glass, outer radius of water
    float wall_thickness  = 0.1f;	// thickness of the glass wall
    float base_thickness  = 0.3f;	// thickness of the glass base
    float water_height    = 1.5f;	// height of water from bottom of glass base on (x, z) plane
    float meniscus_radius = 0.1f;

    enum InputID
    {
        ID_GLASS_AIR_MATERIAL   = 1,
        ID_WATER_AIR_MATERIAL   = 2,
        ID_WATER_GLASS_MATERIAL = 3,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // GlassOfWater

class CutCube : public GeoPrimitive
{
public:
    CutCube() : GeoPrimitive("CutCube") {}

    sm::vec3 p0 = sm::vec3(-1, -1, -1);
    sm::vec3 p1 = sm::vec3(1, 1, 1);

    float radius = 1.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // CutCube

class ConcaveLens : public GeoPrimitive
{
public:
    ConcaveLens() : GeoPrimitive("ConcaveLens") {}

    float radius       = 0;
    float thickness    = 0;
    float min_distance = 0;

    RTTR_ENABLE(GeoPrimitive)

}; // ConcaveLens

class ProductJar : public GeoPrimitive
{
public:
    ProductJar()
        : GeoPrimitive("ProductJar")
    {
        AddPin(std::make_shared<bp::Pin>(true, ID_BODY_MATERIAL, PIN_MATERIAL, "Body Material", *this));

        Layout();
    }

    float bottom              = 0;
    float body_top            = 0;
    float cap_top             = 0;
    float body_radius         = 0;
    float bottom_bevel_radius = 0;
    float top_bevel_radius    = 0;
    float cap_bevel_radius    = 0;

    enum InputID
    {
        ID_BODY_MATERIAL = 1,
    };

    RTTR_ENABLE(GeoPrimitive)

}; // ProductJar

class ConcaveHemisphere : public GeoPrimitive
{
public:
    ConcaveHemisphere() : GeoPrimitive("ConcaveHemisphere") {}

    sm::vec3 center;
    float radius = 1.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // ConcaveHemisphere

class CutFace : public GeoPrimitive
{
public:
    CutFace() : GeoPrimitive("CutFace") {}

    float size   = 1;		// x and w dimensions of the square
    float radius = 0.5;		// radius of the hole

    RTTR_ENABLE(GeoPrimitive)

}; // CutFace

class BeveledBoxShell : public GeoPrimitive
{
public:
    BeveledBoxShell() : GeoPrimitive("BeveledBoxShell") {}

    sm::vec3 bottom;
    sm::vec3 top;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(GeoPrimitive)

}; // BeveledBoxShell

}
}