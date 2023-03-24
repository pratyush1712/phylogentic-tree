#ifndef GENE_HPP
#define GENE_HPP
#include <string>

class Gene
{
public:
    Gene(const std::string sequence);
    bool operator<(const Gene &other) const;
    const std::string &sequence() const;
    int distance(const Gene &other) const;

private:
    std::string seq;
    int recursive_distance(const Gene &a, const Gene &b) const;
};

#endif