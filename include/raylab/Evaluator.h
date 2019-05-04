#pragma once

namespace rt { class World; }

namespace raylab
{

namespace node { class World; }

class Evaluator
{
public:
    static void Build(rt::World& dst, const node::World& src);

}; // Evaluator

}