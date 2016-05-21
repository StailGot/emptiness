#include "emptiness_pch.hpp"

#include "options.hpp"
#include "application.hpp"

#include <iostream>

int main( int argc, char * argv [] )
{
  ::setlocale( 0, "" );

  auto shaders_path = std::tr2::sys::path { sys::options::parse_options( argc, argv, "shaders" ) };
  using application::application_t;
  
  application_t app;
  app
    .init()
    .add_listener( application_t::event_t::draw,   [&app] ( ... ) { app.draw(); } )
    .add_listener( application_t::event_t::close,  [&app] ( ... ) { std::exit( EXIT_SUCCESS ); } )
    .add_listener( application_t::event_t::resize, [&app] ( application_t::event_data_t data ) { ::glViewport(0,0, data.size.x, data.size.y); } )
    
    .add_listener( application_t::event_t::mouse_move
                   , [&app] ( application_t::event_data_t data ) 
                    { 
                      std::cout 
                        << data.mouse.position.x 
                        << " " 
                        << data.mouse.position.y 
                        << "\n"
                        ;
                    } )

    .set_sahders_path( shaders_path.string() )
    .run()
    ;

  return 0;
}
