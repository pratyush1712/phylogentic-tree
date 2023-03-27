#ifndef GENE_HPP
#define GENE_HPP
#include <string>
#include <map>

class Gene
{
public:
    Gene(const std::string sequence);
    bool operator<(const Gene &other) const;
    const std::string &sequence() const;
    int distance(const Gene &other) const;
    int get_id() const;

private:
    static int next_id;
    int id;
    std::string seq;
    int recursive_distance(const std::string &a, const std::string &b) const;
};
extern int GDists[250][250];
extern std::map<std::string, int> existing;
#endif