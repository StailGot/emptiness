#include "emptiness_pch.hpp"

#include "folder_monitor.hpp"
#include "options.hpp"

#include <iostream>
#include <filesystem>
#include <memory>

#include <system\graphics\ogl\ogl.hpp>
#include <system\graphics\window\window.hpp>

int main( int argc, char * argv [] )
{
  auto path =
    std::tr2::sys::path( sys::options::parse_options( argc, argv, "shaders" ) );

  sys::folder_monitor fm { path, [] ( auto && ) { std::cout << 123 << "\n"; } };
  fm.start();

  std::cout << sys::options::parse_options( argc, argv, "shaders" ) << "\n";

  auto wnd = sys::graphics::window::make_window();
  wnd->
    create()
    .set_size( 600, 500 )
    .set_position( 100, 100 )
    .set_title( L"render.window" )
    .show()
    ;

  auto ctx = sys::graphics::ogl::make_gl_context();
  ctx->create( wnd->get_native_handle() ).make_current();

  std::cout << ::glewInit() << "\n";

  sys::graphics::window::message_loop( [&ctx]
  {
    ctx->make_current();
    ::glClearColor( 1, 0.5, ::rand() % 255 / 255., 1 );
    ::glClear( GL_COLOR_BUFFER_BIT );
    ctx->swap_buffers();
  } );

  return 0;
}
