#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Object : public Node
{
public:
    Object(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(true,  0, PIN_MATERIAL, "Material", *this));
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_OBJECT,   "Out", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Object

class Box : public Object
{
public:
    Box() : Object("Box") {}

    sm::vec3 min;
    sm::vec3 max;

    RTTR_ENABLE(Object)

}; // Box

class Sphere : public Object
{
public:
    Sphere() : Object("Sphere") {}

    sm::vec3 center;
    float radius = 1.0f;

    RTTR_ENABLE(Object)

}; // Sphere

class Plane : public Object
{
public:
    Plane() : Object("Plane") {}

    sm::vec3 pos;
    sm::vec3 normal = sm::vec3(0, 1, 0);

    RTTR_ENABLE(Object)

}; // Plane

class Rectangle : public Object
{
public:
    Rectangle()
        : Object("Rectangle")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_SAMPLER,    "Sampler", *this));
        AddPin(std::make_shared<bp::Pin>(true, 2, bp::PIN_FLOAT3, "P0",      *this));
        Layout();
    }

    sm::vec3 p0;
    sm::vec3 a, b;
    sm::vec3 normal = sm::vec3(0, 1, 0);

    enum InputID
    {
        ID_SAMPLER = 1,
        ID_P0,
    };

    RTTR_ENABLE(Object)

}; // Rectangle

class Triangle : public Object
{
public:
    Triangle() : Object("Triangle") {}

    sm::vec3 v0, v1, v2;
    sm::vec3 normal = sm::vec3(0, 1, 0);

    RTTR_ENABLE(Object)

}; // Triangle

class WireframeBox : public Object
{
public:
    WireframeBox() : Object("WireframeBox") {}

    sm::vec3 p0, p1;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(Object)

}; // WireframeBox

class Instance : public Object
{
public:
    Instance() : Object("Instance")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_OBJECT, "Object", *this));
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

    enum InputID
    {
        ID_OBJECT = 1,
    };

    RTTR_ENABLE(Object)

}; // Instance

class Disk : public Object
{
public:
    Disk()
        : Object("Disk")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, PIN_SAMPLER,  "Sampler",  *this));

        Layout();
    }

    enum InputID
    {
        ID_SAMPLER = 1,
    };

    sm::vec3 center;
    sm::vec3 normal = sm::vec3(0, 0, 1);
    float    radius = 1.0;

    bool shadows = false;

    RTTR_ENABLE(Object)

}; // Disk

class SolidCylinder : public Object
{
public:
    SolidCylinder()
        : Object("SolidCylinder")
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

    RTTR_ENABLE(Object)

}; // SolidCylinder

class ConvexPartCylinder : public Object
{
public:
    ConvexPartCylinder() : Object("ConvexPartCylinder") {}

    float bottom    = 0;
    float top       = 1;
    float radius    = 1;
    float polar_min = 0;
    float polar_max = 180;

    RTTR_ENABLE(Object)

}; // ConvexPartCylinder

class OpenPartCylinder : public Object
{
public:
    OpenPartCylinder() : Object("OpenPartCylinder") {}

    float bottom    = 0;
    float top       = 1;
    float radius    = 1;
    float polar_min = 0;
    float polar_max = 180;

    RTTR_ENABLE(Object)

}; // OpenPartCylinder

class Grid : public Object
{
public:
    Grid()
        : Object("Grid")
    {
        AddPin(std::make_shared<bp::Pin>(true, 1, bp::PIN_ANY_VAR, "Children", *this));

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
    TriangleType triangle_type;

    enum InputID
    {
        ID_CHILDREN = 1,
    };

    RTTR_ENABLE(Object)

}; // Grid

class ConcaveSphere : public Object
{
public:
    ConcaveSphere() : Object("ConcaveSphere") {}

    sm::vec3 center;
    float    radius = 1.0f;

    bool shadows = false;

    RTTR_ENABLE(Object)

}; // ConcaveSphere

class Torus : public Object
{
public:
    Torus() : Object("Torus") {}

    float swept_radius = 2.0f;
    float tube_radius  = 0.5f;

    RTTR_ENABLE(Object)

}; // Torus

class OpenCone : public Object
{
public:
    OpenCone() : Object("OpenCone") {}

    float height = 2.0f;
    float radius = 1.5f;

    RTTR_ENABLE(Object)

}; // OpenCone

class ConvexPartSphere : public Object
{
public:
    ConvexPartSphere() : Object("ConvexPartSphere") {}

    sm::vec3 center;
    float radius      = 1.0f;
    float azimuth_min = 0;
    float azimuth_max = 360;
    float polar_min   = 0;
    float polar_max   = 180;

    RTTR_ENABLE(Object)

}; // ConvexPartSphere

class FlatRimmedBowl : public Object
{
public:
    FlatRimmedBowl() : Object("FlatRimmedBowl") {}

    float inner_radius = 0.9f;		// radius of the inside glass surface
    float outer_radius = 1.0f;

    RTTR_ENABLE(Object)

}; // FlatRimmedBowl

class RoundRimmedBowl : public Object
{
public:
    RoundRimmedBowl() : Object("RoundRimmedBowl") {}

    float inner_radius   = 0.9f;		// radius of the inside glass surface
    float outer_radius   = 1.0f;
    float openning_angle = 180;

    RTTR_ENABLE(Object)

}; // RoundRimmedBowl

class ThickRing : public Object
{
public:
    ThickRing()
        : Object("ThickRing")
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

    RTTR_ENABLE(Object)

}; // ThickRing

class BeveledCylinder : public Object
{
public:
    BeveledCylinder() : Object("BeveledCylinder") {}

    float bottom       = 0.0f;
    float top          = 0.0f;
    float radius       = 0.0f;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(Object)

}; // BeveledCylinder

class BeveledRing : public Object
{
public:
    BeveledRing() : Object("BeveledRing") {}

    float bottom       = 0.0f;
    float top          = 0.0f;
    float inner_radius = 0.0f;
    float outer_radius = 0.0f;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(Object)

}; // BeveledRing

class BeveledBox : public Object
{
public:
    BeveledBox() : Object("BeveledBox") {}

    sm::vec3 bottom;
    sm::vec3 top;
    float bevel_radius = 0.0f;

    RTTR_ENABLE(Object)

}; // BeveledBox

class BeveledWedge : public Object
{
public:
    BeveledWedge() : Object("BeveledWedge") {}

    float min_y        = 0;
    float max_y        = 0;
    float inner_radius = 0;
    float outer_radius = 0;
    float bevel_radius = 0;
    float min_azimuth  = 0;
    float max_azimuth  = 0;

    RTTR_ENABLE(Object)

}; // BeveledWedge

}
}