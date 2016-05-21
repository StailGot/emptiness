#include "options.hpp"

#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>


namespace sys{ namespace options {
  std::string parse_options( int argc, char * argv [], const std::string & name )
  {
    namespace po = boost::program_options;

    po::options_description desc { "Allowed options" };
    desc.add_options()
      (name.c_str(), po::value<std::string>()->default_value( "shaders" ), "shaders path")
      ;

    po::variables_map vm;

    po::store( po::command_line_parser( argc, argv )
               .options( desc )
               .allow_unregistered()
               .run()
               , vm );
    po::notify( vm );

    return vm[name].as<std::string>();
  }
}}