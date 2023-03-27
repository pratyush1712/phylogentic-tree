#include "genes_collector.hpp"
#include <algorithm>
#include <atomic>
#include <fstream>
#include <iostream>
#include <mutex>
#include <regex>
#include <thread>
#include <vector>
#include "utils.hpp"
#include <set>
#include <map>
#include <queue>

genesCollector::genesCollector(const std::string &dir) : dir(dir), species(*(new std::vector<Species>())), genes(*(new std::set<Gene>())) {}
int GDists[250][250];
int SDists[40][40];
std::map<std::string, int> existing;
int Gene::next_id = -1;

void genesCollector::compute()
{
    std::vector<fs::path> files_to_sweep = utils::find_all_files(dir, [](const std::string &extension)
                                                                 { return extension == ".dat"; });
    std::atomic<uint64_t> global_index = 0;

    // std::set<Gene> genes;
    // std::map<Species, std::set<Gene>> file_to_genes;
    // std::set<Species> species;

    uint64_t file_index;
    while ((file_index = global_index++) < files_to_sweep.size())
    {
        process_file(files_to_sweep[file_index], file_index);
    }
}

void genesCollector::display()
{
    // sort the genes by length and then alphabetically
    std::vector<Gene> vec_genes(genes.begin(), genes.end());
    std::sort(vec_genes.begin(), vec_genes.end());
    int gene_index = 0;
    for (const auto &gene : vec_genes)
    {
        std::cout << "G" << gene_index << "=" << gene.sequence().c_str() << std::endl;
        gene_index++;
    }
    std::cout << std::endl;

    for (int i = 0; i < vec_genes.size(); i++)
    {
        std::cout << "G" << i << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < vec_genes.size(); i++)
    {
        for (int j = 0; j < vec_genes.size(); j++)
        {
            if (i != j && GDists[vec_genes[j].get_id()][vec_genes[i].get_id()] != 0)
            {
                GDists[vec_genes[i].get_id()][vec_genes[j].get_id()] = GDists[vec_genes[j].get_id()][vec_genes[i].get_id()];
            }
            else
            {
                GDists[vec_genes[i].get_id()][vec_genes[j].get_id()] = vec_genes[i].distance(vec_genes[j]);
            }
            std::cout << GDists[vec_genes[i].get_id()][vec_genes[j].get_id()] << " ";
        }
        std::cout << "// G" << i << std::endl;
    }

    std::cout << std::endl;
    // print file to genes with genes sorted. Example, Armored Snapper: Genes [0, 1, 2, 3, 12, 13, 17]
    for (const auto &file_to_gene : file_to_genes)
    {
        std::cout << "S" << file_to_gene.first.species_index << "=" << file_to_gene.first.get_species_name() << ": Genes [";
        int gene_index = 0;
        int species_gene_index = 0;
        for (const auto &gene : vec_genes)
        {
            if (file_to_gene.second.find(gene) != file_to_gene.second.end()) // gene is in file
            {
                std::cout << gene_index;
                if (species_gene_index != file_to_gene.second.size() - 1)
                {
                    std::cout << ", ";
                }
                species_gene_index++;
            }
            gene_index++;
        }
        std::cout << "]" << std::endl;
    }

    std::cout << std::endl;
    // now printing species distance matrix
    for (int i = 0; i < file_to_genes.size(); i++)
    {
        std::cout << "S" << i << " ";
    }
    std::cout << std::endl;
    for (const auto spec : species)
    {
        for (const auto spec2 : species)
        {
            if (spec.species_index != spec2.species_index && SDists[spec2.species_index][spec.species_index] != 0)
                SDists[spec.species_index][spec2.species_index] = SDists[spec2.species_index][spec.species_index];
            else
                SDists[spec.species_index][spec2.species_index] = spec.distance(spec2);

            std::cout << SDists[spec.species_index][spec2.species_index] << " ";
        }
        std::cout << std::endl;
    }
    // now computing and creating phylogenetic tree
    std::cout << std::endl;
    std::cout << "Phylogenetic Tree:" << std::endl;
    std::cout << std::endl;
    Species &root = species[0];
    std::priority_queue<Edge> queue;
    for (Species &spec : species)
    {
        if (&spec != &root)
        {
            queue.emplace(Edge(root, spec));
        }
    }
    // now add all species to the tree
    std::set<Species *> species_in_tree;
    species_in_tree.insert(&root);
    while (!queue.empty())
    {
        Edge edge = queue.top();
        queue.pop();
        if (species_in_tree.find(&edge.getChild()) == species_in_tree.end())
        {
            species_in_tree.insert(&edge.getChild());
            edge.getParent().add_child(edge.getChild());
            for (Species &spec : species)
            {
                if (species_in_tree.find(&spec) == species_in_tree.end() && &spec != &edge.getChild() && &spec != &edge.getParent() && SDists[spec.species_index][edge.getChild().species_index] != 9999)
                {
                    queue.emplace(Edge(edge.getChild(), spec));
                }
            }
        }
    }
}

void genesCollector::process_file(const fs::path &file, const int file_index)
{
    std::ifstream fin(file);
    std::stringstream buffer;
    buffer << fin.rdbuf();
    std::string contents = buffer.str();
    int index = std::stoi(file.filename().string().substr(1, file.filename().string().size() - 5));
    Species animal(contents, file_index);
    species.emplace_back(animal);
    const auto &genes = animal.get_parsed_genes();
    // sort genes by length and then alphabetically
    std::vector<Gene> vec_genes(genes.begin(), genes.end());
    std::sort(vec_genes.begin(), vec_genes.end());
    for (const auto &gene : vec_genes)
    {
        file_to_genes[animal].insert(gene);
        this->genes.insert(gene);
    }
}
