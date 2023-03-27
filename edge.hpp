#pragma once
#include "species.hpp"

class Edge
{
public:
    Edge(Species &parent, Species &child);
    Edge(const Edge &other);
    Edge &operator=(const Edge &other);

    Species &getParent() const;

    Species &getChild() const;

    int getDistance() const;

    bool operator<(const Edge &other) const;

private:
    Species &parent;
    Species &child;
    int distance;
};
