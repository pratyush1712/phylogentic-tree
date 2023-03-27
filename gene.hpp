#ifndef GENE_HPP
#define GENE_HPP
#include <string>
#include <map>

class Gene
{
public:
    Gene(const std::string sequence);
    int get_id() const;
    const std::string &sequence() const;

    bool operator<(const Gene &other) const;
    int distance(const Gene &other) const;

private:
    std::string seq;
    int id;
    static int next_id;
    static int recursive_distance(const std::string &a, const std::string &b);
};
extern int GDists[250][250];
extern std::map<std::string, int> existing;
#endif