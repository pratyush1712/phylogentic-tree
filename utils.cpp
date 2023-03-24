#include "utils.hpp"
#include "species.hpp"
#include <vector>
#include <map>
#include <string>
#include <list>
#include <iostream>

// Score for each common instance of a character
const int COMMON_COST = 15;
// Score for difference in number of instances public static final int
const int DIFF_NUM_COST = 8;
// Score for characters appearing only in one gene fragment
const int ONLY_IN_ONE_COST = 25;
// Sliding window minimum match length
const int MIN_MATCH = 4;

std::vector<fs::path>
utils::find_all_files(
    const fs::path &dir, std::function<bool(const std::string &)> pred)
{
    std::list<fs::path> files_to_sweep;
    for (auto &entry : fs::recursive_directory_iterator(dir))
    {
        if (entry.is_regular_file())
        {
            fs::path cur_file = entry.path();
            std::string type(cur_file.extension());
            if (pred(type))
            {
                files_to_sweep.push_back(std::move(cur_file));
            }
        }
    }
    return std::vector<fs::path>(std::make_move_iterator(files_to_sweep.begin()),
                                 std::make_move_iterator(files_to_sweep.end()));
}

std::pair<Gene, Gene> utils::trimmed_sequences(const Gene &a, const Gene &b)
{
    size_t i = 0;
    while (i < a.sequence().size() && i < b.sequence().size() && a.sequence()[i] == b.sequence()[i])
    {
        ++i;
    }
    size_t j = 0;
    while (j < a.sequence().size() - i && j < b.sequence().size() - i && a.sequence()[a.sequence().size() - j - 1] == b.sequence()[b.sequence().size() - j - 1])
    {
        ++j;
    }
    Gene first(a.sequence().substr(i, a.sequence().size() - i - j));
    Gene second(b.sequence().substr(i, b.sequence().size() - i - j));
    return std::make_pair(first, second);
}

int utils::calculate_score(const Gene &a, const Gene &b)
{
    if (a.sequence() == b.sequence())
        return 0;
    std::vector<char> chars = {'I', 'Y', 'A', 'T'};
    std::map<char, int> a_map;
    std::map<char, int> b_map;
    for (auto &c : chars)
    {
        a_map[c] = 0;
        b_map[c] = 0;
    }

    for (auto &c : a.sequence())
    {
        a_map[c]++;
    }
    for (auto &c : b.sequence())
    {
        b_map[c]++;
    }

    int score = 0;
    for (auto &c : chars)
    {
        if (a_map[c] == 0 || b_map[c] == 0)
        {
            score += (a_map[c] + b_map[c]) * ONLY_IN_ONE_COST;
        }
        else
        {
            int common = std::min(a_map[c], b_map[c]);
            int diff = std::abs(a_map[c] - b_map[c]);
            score += common * COMMON_COST + diff * DIFF_NUM_COST;
        }
    }
    return score;
}

std::vector<Gene> utils::split_matches(const Gene &a, const Gene &b)
{
    std::vector<Gene> result;
    if (a.sequence().size() < MIN_MATCH || b.sequence().size() < MIN_MATCH)
        return result;
    for (size_t i = 0; i < a.sequence().size() - MIN_MATCH; ++i)
    {
        for (size_t j = 0; j < b.sequence().size() - MIN_MATCH; ++j)
        {
            if (a.sequence().substr(i, MIN_MATCH) == b.sequence().substr(j, MIN_MATCH))
            {
                result.push_back(Gene(a.sequence().substr(0, i))); // G0left
                result.push_back(Gene(b.sequence().substr(0, j))); // G1left
                // now expand the match to as long as possible
                size_t k = i + MIN_MATCH;
                size_t l = j + MIN_MATCH;
                while (k < a.sequence().size() && l < b.sequence().size() && a.sequence()[k] == b.sequence()[l])
                {
                    ++k;
                    ++l;
                }
                // now push the unmatched part of G0 and G1
                result.push_back(Gene(a.sequence().substr(k))); // G0right
                result.push_back(Gene(b.sequence().substr(l))); // G1right
                return result;
            }
        }
    }
    return result;
}