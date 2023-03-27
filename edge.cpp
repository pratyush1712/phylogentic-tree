#include "edge.hpp"

Edge::Edge(Species &parent, Species &child) : parent(parent), child(child)
{
    distance = SDists[parent.species_index][child.species_index];
}

Edge::Edge(const Edge &other) : parent(other.parent), child(other.child) {}

Species &Edge::getParent() const
{
    return parent;
}

Edge &Edge::operator=(const Edge &other)
{
    if (this != &other)
    {
        parent = other.getParent();
        child = other.getChild();
    }
    return *this;
}

Species &Edge::getChild() const
{
    return child;
}

int Edge::getDistance() const
{
    return distance;
}

bool Edge::operator<(const Edge &other) const
{
    return distance < other.getDistance() || (distance == other.getDistance() && child.get_species_name() < other.getChild().get_species_name());
}