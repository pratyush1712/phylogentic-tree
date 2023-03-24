#include <filesystem>
#include <functional>
#include <vector>
#include "species.hpp"

namespace fs = std::filesystem;

namespace utils
{
    std::vector<fs::path> find_all_files(
        const fs::path &dir, std::function<bool(const std::string &)> pred);

    std::pair<Gene, Gene> trimmed_sequences(const Gene &a, const Gene &b);

    int calculate_score(const Gene &a, const Gene &b);

    std::vector<Gene> split_matches(const Gene &a, const Gene &b);
}