#include <filesystem>
#include <set>
#include <map>
#include <vector>
#include <string>
#include "utils.hpp"

namespace fs = std::filesystem;

class genesCollector
{
    // main storage structure for word frequencies
    std::set<Gene> &genes;
    std::map<Species, std::set<Gene>> file_to_genes;
    fs::path dir;
    std::set<Species> species;

    void process_file(const fs::path &file, const int file_index);

public:
    genesCollector(const std::string &dir);
    void compute();
    void display();
};
