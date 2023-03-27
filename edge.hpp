#pragma once
#include "species.hpp"

class Edge
{
public:
    Edge(Species &parent, Species &child);

    Species &getParent() const;

    Species &getChild() const;

    int getDistance() const;

private:
    Species &parent;
    Species &child;
    int distance;
};
