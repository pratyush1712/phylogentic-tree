#include <queue>
#include <set>
#include <iostream>
#include "species.hpp"
#include "edge.hpp"

class Tree
{
public:
    explicit Tree(const std::vector<Species> &species) : species_(species) {}

    void create_phylogenetic_tree(int root_index)
    {
        Species &root = species_[root_index];
        int num_species = species_.size();
        std::priority_queue<Edge> queue;
        for (Species &spec : species_)
        {
            if (&spec != &root)
            {
                queue.emplace(Edge(root, spec));
            }
        }

        std::set<Species *> species_in_tree;
        species_in_tree.insert(&root);
        while (!queue.empty() && num_species > 1)
        {
            Edge edge = queue.top();
            queue.pop();
            if (species_in_tree.find(&edge.getChild()) == species_in_tree.end())
            {
                species_in_tree.insert(&edge.getChild());
                num_species--;
                edge.getParent().children.push_back(&edge.getChild());
                for (Species &spec : species_)
                {
                    if (species_in_tree.find(&spec) == species_in_tree.end() && &spec != &edge.getChild() && &spec != &edge.getParent() && SpeciesMemo[spec.species_index][edge.getChild().species_index] != 9999)
                    {
                        queue.emplace(Edge(edge.getChild(), spec));
                    }
                }
            }
        }
    }

    void print_phylogenetic_tree() const
    {
        species_[0].print_tree(0);
    }

private:
    std::vector<Species> species_;
};