#include "edge.hpp"

Edge::Edge(Species &parent, Species &child) : parent(parent), child(child) {}

Species &Edge::getParent() const
{
    return parent;
}

Species &Edge::getChild() const
{
    return child;
}

int Edge::getDistance() const
{
    return distance;
}