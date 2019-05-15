#pragma once

#include <SM_Vector.h>

#include <memory>

namespace rt
{
    class World;
    class Light;
    class Tracer;
    class Object;
    class Material;
    class Camera;
    class Texture;
    class BRDF;
    class Sampler;
    class Mapping;
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

    static std::unique_ptr<rt::Object>
        CreateObject(const bp::Node& node);

    static std::shared_ptr<rt::Material>
        CreateMaterial(const bp::Node& node);

    static std::unique_ptr<rt::Camera>
        CreateCamera(const bp::Node& node);

    static std::shared_ptr<rt::Texture>
        CreateTexture(const bp::Node& node);

    static std::shared_ptr<rt::BRDF>
        CreateBRDF(const bp::Node& node);

    static std::shared_ptr<rt::Sampler>
        CreateSampler(const bp::Node& node);

    static std::shared_ptr<rt::Mapping>
        CreateMapping(const bp::Node& node);

    static float CalcFloat(const bp::Connecting& conn);
    static sm::vec3 CalcFloat3(const bp::Connecting& conn);

}; // Evaluator

}