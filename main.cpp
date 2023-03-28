#include <iostream>
#include "genes_collector.hpp"

// this program computes word frequencies for all .h and .c files in the given directory and its subdirectories
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <dir>" << std::endl;
    return 1;
  }
  // std::pair<std::string, std::string> results = utils::trimmed_sequences(argv[1], argv[2]);
  // // print the results pair
  // std::cout << "trimmed sequences: " << std::endl;
  // std::cout << "first: " << results.first << std::endl;
  // std::cout << "second: " << results.second << std::endl;
  // // first split into matches
  // std::vector<std::string> matches = utils::split_matches(results.first, results.second);
  // // print the matches
  // std::cout << "matches: " << std::endl;
  // for (auto &m : matches)
  //   std::cout << m << std::endl;

  genesCollector genes_collector(argv[1]);
  genes_collector.compute();
  genes_collector.display();
}
