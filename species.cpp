#include <iostream>
#include <string>
#include <set>
#include "species.hpp"
#include <vector>

const int MAX_DISTANCE = 9999;

Species::Species(const std::string &content, int file_index) : species_index(file_index)
{
    std::string name = content.substr(content.find("Name=") + 6);
    this->species_name = name.substr(0, name.find("\""));

    std::string latin_name = content.substr(content.find("LatinName=") + 11);
    this->latin_name = latin_name.substr(0, latin_name.find("\""));

    std::string image = content.substr(content.find("ImageFileName=") + 15);
    this->image_file_name = image.substr(0, image.find("\""));

    this->dna = content.substr(content.find("DNA=") + 4);

    parse_genes();
}

bool Species::operator<(const Species &other) const
{
    return this->species_index < other.species_index;
}

void Species::parse_genes()
{
    std::string start_delimiter = "IAY";
    std::string end_delimiter = "TYI";
    size_t start_pos = 0;
    while ((start_pos = dna.find(start_delimiter, start_pos)) != std::string::npos)
    {
        start_pos += start_delimiter.length();
        size_t end_pos = dna.find(end_delimiter, start_pos);
        if (end_pos == std::string::npos)
        {
            break;
        }

        std::string gene_sequence = dna.substr(start_pos, end_pos - start_pos);
        if (existing.find(gene_sequence) == existing.end())
        {
            Gene gene(gene_sequence);
            existing[gene_sequence] = gene.get_id();
            genes.insert(gene);
        }
        start_pos = end_pos + end_delimiter.length();
    }
}

const std::set<Gene> &Species::get_parsed_genes() const
{
    return this->genes;
}

const std::string &Species::get_species_name() const
{
    return this->species_name;
}

const std::string &Species::get_latin_name() const
{
    return this->latin_name;
}

int Species::distance(const Species &other) const
{
    if (this->species_name == other.get_species_name())
    {
        return 0;
    }
    // if sibling species, return max distance
    if (this->is_sibling(other))
    {
        return MAX_DISTANCE;
    }
    int distance_right = 0;
    // find the closest gene in the other species
    for (const Gene &gene : this->genes)
    {
        int gene_id = gene.get_id();
        int min_distance = MAX_DISTANCE;
        for (const Gene &other_gene : other.genes)
        {
            int distance = GeneMemo[gene_id][other_gene.get_id()];
            if (distance < min_distance)
            {
                min_distance = distance;
            }
        }
        distance_right += min_distance;
    }
    // now in the other direction
    int distance_left = 0;
    for (const Gene &gene : other.get_parsed_genes())
    {
        int gene_id = gene.get_id();
        int min_distance = MAX_DISTANCE;
        for (const Gene &other_gene : this->genes)
        {
            int distance = GeneMemo[gene_id][other_gene.get_id()];
            if (distance < min_distance)
            {
                min_distance = distance;
            }
        }
        distance_left += min_distance;
    }
    int final_distance = (distance_left + distance_right) / 2;
    SpeciesMemo[this->species_index][other.species_index] = final_distance;
    return final_distance;
}

bool Species::is_sibling(const Species &other) const
{
    if (this->species_name == other.get_species_name())
    {
        return true;
    }
    for (const Gene &gene : this->genes)
    {
        if (other.get_parsed_genes().count(gene) > 0)
        {
            return true;
        }
    }
    return false;
}

void Species::add_child(Species &child)
{
}

const std::vector<Species *> &Species::get_children() const
{
    return this->children;
}

bool Species::operator!=(const Species &other) const
{
    return this->species_name != other.get_species_name();
}

void Species::print_children(int indent_level) const
{
    std::string indent(indent_level, '\t');
    std::cout << indent << "- Species " << species_index << std::endl;
    for (const auto &child : children)
    {
        child->print_children(indent_level + 1);
    }
}

void Species::print_tree(int indent_level) const
{
    std::string indent(indent_level, '\t');
    std::cout << indent << species_name << std::endl;
    for (const auto &child : children)
    {
        child->print_tree(indent_level + 1);
    }
}