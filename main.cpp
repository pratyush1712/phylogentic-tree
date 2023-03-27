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

  genesCollector genes_collector(argv[1]);
  genes_collector.compute();
  genes_collector.display();
}
