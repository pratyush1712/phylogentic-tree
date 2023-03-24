#include "utils.hpp"
#include <algorithm>
#include <set>
#include <vector>
#include <map>
#include <queue>

class Node
{

public:
    std::vector<Node> children;
    Species species;
    Node(Species species) : species(species), children({}) {}

    void add_child(Node child)
    {
        children.push_back(child);
    }
    std::vector<Node> &get_children()
    {
        return children;
    }

    Species &get_species()
    {
        return species;
    }
    bool operator<(const Node &other) const
    {
        return species.get_species_name() < other.species.get_species_name();
    }
};

class Edge
{
private:
    int distance;

public:
    Node &from;
    Node &to;
    Edge(Node &from, Node &to, int distance) : from(from), to(to), distance(distance) {}
    bool operator<(const Edge &other) const
    {
        if (this->distance == other.distance)
        {
            return to < other.to;
        }
        return distance < other.distance;
    }

    int get_distance()
    {
        return this->distance;
    }
    // assignment operator
    Edge &operator=(const Edge &other)
    {
        this->distance = other.distance;
        this->from = other.from;
        this->to = other.to;
        return *this;
    }
};

class PhylogenyTree
{
private:
    std::set<Node> &animals;
    Node &root;
    std::vector<Edge> &edges;

public:
    PhylogenyTree(std::set<Species> &species, Species &root) : animals(), root(Node(root))
    {
        this->animals.insert(Node(root));
        edges.push_back({});

        std::priority_queue<Edge> edges_to_add;
        for (auto &spec : species)
        {
            edges_to_add.push(Edge(Node(root), Node(spec), root.distance(spec)));
        }
        while (!edges_to_add.empty())
        {
            Edge edge = edges_to_add.top();
            edges_to_add.pop();
            // get the node that is in the tree and that is not in the tree
            if (animals.find(edge.from) != animals.end() && animals.find(edge.to) != animals.end())
            {
                continue;
            }
            edges.push_back(edge);
            animals.insert(edge.to);
            // add new species to children of parent
            edge.from.add_child(edge.to);

            for (auto &spec : species)
            {
                // if species is not sibling of edge.to
                if (spec.is_sibling(edge.to.get_species()) == false)
                {
                    edges_to_add.push(Edge(edge.to, Node(spec), edge.to.get_species().distance(spec)));
                }
            }
        }
    }
};