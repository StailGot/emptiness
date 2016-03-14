#pragma once

#include <string>


namespace options
{
  std::string parse_options( int argc, char * argv [], const std::string & name );
}