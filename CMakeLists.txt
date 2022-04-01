cmake_minimum_required(VERSION 3.0)
project(dbds)

include_directories(/home/user/llvm-v10/include /usr/local/opt/llvm/include /Users/kaifranz/CLionProjects/llvm-project/build/include)

set(CMAKE_CXX_STANDARD 14)

add_executable(DbdsPass DbdsPass.cpp SimulatedOptimization.h SimulationResult.cpp SimulationResult.h ConstantFolding.h)
