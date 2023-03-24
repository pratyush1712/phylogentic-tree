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
  // sample code to test utils::trimmed_sequences
  // std::pair<Gene, Gene> trimmed = utils::trimmed_sequences(Gene(argv[1]), Gene(argv[2]));
  // std::cout << "trimmed: " << trimmed.first.sequence() << " " << trimmed.second.sequence() << std::endl;
  // // sample code to test utils::calculate_score
  // const auto split_matches = utils::split_matches(Gene(argv[1]), Gene(argv[2]));
  // for (const auto &gene : split_matches)
  // {
  //   std::cout << gene.sequence() << std::endl;
  // }

  genesCollector genes_collector(argv[1]);
  genes_collector.compute();
  genes_collector.display();
}
