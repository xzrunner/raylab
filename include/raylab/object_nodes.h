#pragma once

#include "raylab/Node.h"

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
        AddPin(std::make_shared<bp::Pin>(true,  0, bp::PIN_ANY_VAR, "Material", *this));
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

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
    float radius = 0.0f;

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
        AddPin(std::make_shared<bp::Pin>(true, 1, bp::PIN_ANY_VAR, "Sampler", *this));
        Layout();
    }

    sm::vec3 p0;
    sm::vec3 a, b;
    sm::vec3 normal = sm::vec3(0, 1, 0);

    enum InputID
    {
        ID_SAMPLER = 1,
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

}
}