#include <string>
#include <set>
#include <iostream>
#include <tuple>
#include "gene.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>

Gene::Gene(const std::string sequence) : seq(sequence), id(next_id++) {}

int Gene::get_id() const
{
    return id;
}

const std::string &Gene::sequence() const
{
    return this->seq;
}

bool Gene::operator<(const Gene &other) const
{
    return seq.size() < other.sequence().size() || (seq.size() == other.sequence().size() && seq < other.sequence());
}

int Gene::distance(const Gene &other) const
{
    auto [G0, G1] = utils::trimmed_sequences(seq, other.sequence());
    if (G0.empty() && G1.empty())
        return 0;
    return std::min(recursive_distance(G0, G1), recursive_distance(G1, G0));
}

int Gene::recursive_distance(const std::string &a, const std::string &b) const
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