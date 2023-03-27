#!/bin/bash

# g++ version must be >=8 
g++-8 -O3 -std=c++17 main.cpp genes_collector.cpp species.cpp gene.cpp utils.cpp edge.cpp -lpthread -lstdc++fs -o ptree
