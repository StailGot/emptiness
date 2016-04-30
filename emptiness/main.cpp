#include "emptiness_pch.hpp"

#include "folder_monitor.hpp"
#include "options.hpp"

#include <iostream>
#include <filesystem>
#include <thread>



int main( int argc, char * argv [] )
{
  auto path = std::tr2::sys::path( sys::options::parse_options( argc, argv, "shaders" ) );  

  sys::folder_monitor fm{ path, [] (auto &&) { std::cout << 123 << "\n"; } };
  fm.start(); 

  std::cout << sys::options::parse_options( argc, argv, "shaders" ) << "\n";
  std::getchar();
  return 0;
}