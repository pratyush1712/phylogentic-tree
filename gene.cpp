#include "gene.hpp"
#include "utils.hpp"
#include <algorithm>

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
