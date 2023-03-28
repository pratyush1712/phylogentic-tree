#ifndef GENE_HPP
#define GENE_HPP
#include <string>
#include <map>
#include "utils.hpp"

class Gene
{
public:
    Gene(const std::string sequence);
    int get_id() const;
    const std::string &sequence() const;

    bool operator<(const Gene &other) const;
    inline int distance(const Gene &other) const
    {
        auto [G0, G1] = utils::trimmed_sequences(seq, other.sequence());
        if (G0.empty() && G1.empty())
            return 0;
        return std::min(recursive_distance(G0, G1), recursive_distance(G1, G0));
    }

private:
    std::string seq;
    int id;
    static int next_id;
    static inline int recursive_distance(const std::string &a, const std::string &b)
    {
        const int G0left = 0;
        const int G1left = 1;
        const int G0right = 2;
        const int G1right = 3;
        auto genes = utils::split_matches(a, b);
        if (genes.size() == 0)
            return utils::calculate_score(a, b);
        int score_left = utils::calculate_score(genes[G0left], genes[G1left]);
        int score_right = std::min(Gene::recursive_distance(genes[G0right], genes[G1right]),
                                   Gene::recursive_distance(genes[G1right], genes[G0right]));
        return score_left + score_right;
    }
};
extern int GeneMemo[250][250];
extern std::map<std::string, int> existing;
#endif