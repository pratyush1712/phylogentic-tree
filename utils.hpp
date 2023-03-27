#ifndef UTILS_HPP
#define UTILS_HPP
#include <filesystem>
#include <functional>
#include <vector>
#include <string>
#include <map>

namespace fs = std::filesystem;

// Score for each common instance of a character
constexpr int COMMON_COST = 15;
// Score for difference in number of instances public static final int
constexpr int DIFF_NUM_COST = 8;
// Score for characters appearing only in one gene fragment
constexpr int ONLY_IN_ONE_COST = 25;
// Sliding window minimum match length
constexpr int MIN_MATCH = 4;

namespace utils
{
    std::vector<fs::path> find_all_files(
        const fs::path &dir, std::function<bool(const std::string &)> pred);

    inline std::pair<std::string, std::string> trimmed_sequences(const std::string &a, const std::string &b)
    {
        size_t i = 0;
        while (i < a.size() && i < b.size() && a[i] == b[i])
        {
            ++i;
        }
        size_t j = 0;
        while (j < a.size() - i && j < b.size() - i && a[a.size() - j - 1] == b[b.size() - j - 1])
        {
            ++j;
        }
        std::string first(a.substr(i, a.size() - i - j));
        std::string second(b.substr(i, b.size() - i - j));
        return std::make_pair(first, second);
    }

    inline int _calculate_score(const std::string &a, const std::string &b)
    {
        if (a == b)
            return 0;
        std::vector<char> chars = {'I', 'Y', 'A', 'T'};
        std::map<char, int> a_map;
        std::map<char, int> b_map;

        for (auto &c : a)
            a_map[c]++;
        for (auto &c : b)
            b_map[c]++;

        int score = 0;
        for (auto &c : chars)
        {
            if (a_map[c] == 0 || b_map[c] == 0)
                score += std::max(a_map[c], b_map[c]) * ONLY_IN_ONE_COST;
            else
            {
                int common = std::min(a_map[c], b_map[c]);
                int diff = std::abs(a_map[c] - b_map[c]);
                score += common * COMMON_COST + diff * DIFF_NUM_COST;
            }
        }
        return score;
    }

    inline int calculate_score(const std::string &a, const std::string &b)
    {
        return std::min(_calculate_score(a, b), _calculate_score(b, a));
    }

    inline std::vector<std::string> split_matches(const std::string &a, const std::string &b)
    {
        std::vector<std::string> result;
        if (a.size() < MIN_MATCH || b.size() < MIN_MATCH)
            return result;
        for (size_t i = 0; i < a.size() - MIN_MATCH; ++i)
        {
            for (size_t j = 0; j < b.size() - MIN_MATCH; ++j)
            {
                if (a.substr(i, MIN_MATCH) == b.substr(j, MIN_MATCH))
                {
                    result.push_back(a.substr(0, i)); // G0left
                    result.push_back(b.substr(0, j)); // G1left
                    // now expand the match to as long as possible
                    size_t k = i + MIN_MATCH;
                    size_t l = j + MIN_MATCH;
                    while (k < a.size() && l < b.size() && a[k] == b[l])
                    {
                        ++k;
                        ++l;
                    }
                    // now push the unmatched part of G0 and G1
                    result.push_back(a.substr(k)); // G0right
                    result.push_back(b.substr(l)); // G1right
                    return result;
                }
            }
        }
        return result;
    }
}

#endif // UTILS_HPP
