cmake_minimum_required(VERSION 3.10)
project(project_name)

set(CMAKE_CXX_STANDARD 17)

# Add your source files here
set(source_files 
    main.cpp 
    genes_collector.cpp 
    species.cpp 
    gene.cpp 
    utils.cpp 
    edge.cpp)

add_executable(ptree ${source_files} ${root_index})

# Link any required libraries
target_link_libraries(ptree pthread stdc++fs)
