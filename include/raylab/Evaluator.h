#pragma once

#include <SM_Vector.h>

#include <memory>
#include <vector>

namespace rt
{
    class World;
    class Light;
    class Tracer;
    class GeoPrimitive;
    class Material;
    class Camera;
    class Texture;
    class BRDF;
    class Sampler;
    class Mapping;
    class LatticeNoise;
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
    static std::unique_ptr<rt::Light>
        CreateLight(const bp::Node& node);

    static std::unique_ptr<rt::Tracer>
        CreateTracer(const bp::Node& node, rt::World& dst);

    static void CreateObjects(const bp::Connecting& conn,
        std::vector<std::unique_ptr<rt::GeoPrimitive>>& objects);
    static std::unique_ptr<rt::GeoPrimitive>
        CreateObject(const bp::Node& node);

    static std::unique_ptr<rt::Material>
        CreateMaterial(const bp::Node& node);

    static std::unique_ptr<rt::Camera>
        CreateCamera(const bp::Node& node);

    static std::unique_ptr<rt::Texture>
        CreateTexture(const bp::Node& node);

    static std::unique_ptr<rt::BRDF>
        CreateBRDF(const bp::Node& node);

    static std::unique_ptr<rt::Sampler>
        CreateSampler(const bp::Node& node);

    static std::unique_ptr<rt::Mapping>
        CreateMapping(const bp::Node& node);

    static std::unique_ptr<rt::LatticeNoise>
        CreateNoise(const bp::Node& node);

}; // Evaluator

}