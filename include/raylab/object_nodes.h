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

class SolidCylinder : public Object
{
public:
    SolidCylinder() : Object("SolidCylinder") {}

    float bottom = 0;
    float top    = 0;
    float radius = 0;

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

class Grid : public Object
{
public:
    Grid() : Object("Grid") {}

    std::string filename;

    enum class TriangleType
    {
        Flat,
        Smooth
    };
    TriangleType triangle_type;

    RTTR_ENABLE(Object)

}; // Grid

}
}