#pragma once

#include <memory>

namespace rt
{
    class World;
    class Object;
    class Material;
}

namespace bp
{
    class Node;
    class Connecting;
}

namespace raylab
{

namespace node { class World; }

class Evaluator
{
public:
    static void Build(rt::World& dst, const node::World& src);

private:
    static std::unique_ptr<rt::Object>
        CreateObject(const bp::Node& node);

    static std::shared_ptr<rt::Material>
        CreateMaterial(const bp::Node& node);

    static float CalcFloat(const bp::Connecting& conn);

}; // Evaluator

}