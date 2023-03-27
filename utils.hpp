#include <filesystem>
#include <functional>
#include <vector>
#include "species.hpp"
#include "edge.hpp"

namespace fs = std::filesystem;

namespace utils
{
    std::vector<fs::path> find_all_files(
        const fs::path &dir, std::function<bool(const std::string &)> pred);

    std::pair<std::string, std::string> trimmed_sequences(const std::string &a, const std::string &b);

    int calculate_score(const std::string &a, const std::string &b);

    std::vector<std::string> split_matches(const std::string &a, const std::string &b);
}