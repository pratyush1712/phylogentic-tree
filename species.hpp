#ifndef SPECIES_HPP
#define SPECIES_HPP
#include <string>
#include <set>
#include <vector>
#include "gene.hpp"

extern int SDists[40][40];
class Species
{
public:
    int species_index;
    Species(const std::string &file_content, int file_index);
    void parse_genes();
    const std::set<Gene> &get_parsed_genes() const;
    const std::string &get_species_name() const;
    const std::string &get_latin_name() const;
    bool operator<(const Species &other) const;
    int distance(const Species &other) const;
    bool is_sibling(const Species &other) const;
    void add_child(const Species &child);
    const std::vector<Species> &get_children() const;

private:
    std::string species_name;
    std::string latin_name;
    std::string image_file_name;
    std::string dna;
    std::set<Gene> genes;
    std::vector<Species> children;
};

#endif