#pragma once

#include <string>

namespace sys{ namespace options {
  std::string parse_options( int argc, char * argv [], const std::string & name );
}}