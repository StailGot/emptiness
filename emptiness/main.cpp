#include "emptiness_pch.hpp"

#include "folder_monitor.hpp"
#include "options.hpp"

#include <iostream>
#include <filesystem>
#include <memory>
#include <chrono>
#include <string>

#include <boost\timer\timer.hpp>

#include <system\graphics\ogl\ogl.hpp>
#include <system\graphics\window\window.hpp>
#include <system\graphics\ogl\gl_utils.hpp>

#include <concurrent_queue.h>

int main( int argc, char * argv [] )
{
  ::setlocale( 0, "" );

  enum class event: uint32_t { on_shader_change };
  concurrency::concurrent_queue< event > events;

  auto shaders_path = std::tr2::sys::path { sys::options::parse_options( argc, argv, "shaders" ) };
  auto on_shader_change =
    [&events] ( const std::wstring & name )
  {
    //std::wcout << name << "\n"; 
    events.push( event::on_shader_change );
  };

  sys::folder_monitor shader_folder_monitor { shaders_path, on_shader_change };
  shader_folder_monitor.start();

  std::cout << shaders_path << "\n";

  auto window = sys::graphics::window::make_window();
  window->
    create()
    .set_title( L"ogl window" )
    .set_size( 500, 500 )
    .set_position( 500, 500 )
    .show()
    ;

  auto ctx = sys::graphics::ogl::make_gl_context();
  ctx->create( window->get_native_handle() ).make_current();

  ::glewInit();

  namespace gl = sys::graphics::ogl;

  auto make_shader_program =
    [shaders_path = shaders_path.string()]
  {
    auto shaders = gl::load_shaders( shaders_path );
    return gl::create_program( shaders, &std::cout );
  };

  GLuint program = make_shader_program();

  ::glEnable( GL_PROGRAM_POINT_SIZE );

  sys::graphics::window::message_loop
  (
    [&events, &ctx, &program, &make_shader_program]
  {
    GLfloat color [4] = {};
    ::glClearBufferfv( GL_COLOR, 0, color );
    ::glUseProgram( program );

    ::glBegin( GL_POINTS );
    ::glVertex3f( 0, 0, 0 );
    ::glEnd();

    ctx->swap_buffers();

    event e = {};
    if (events.try_pop( e ))
    {
      ::glDeleteProgram( program );
      program = make_shader_program();
      std::cout << "on_shader_change" << "\n";
    }
  }
  );

  return 0;
}
