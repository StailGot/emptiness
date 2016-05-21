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
#include <gsl.h>

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

  using sys::graphics::window::event_t;
  using sys::graphics::window::event_data_t;

  auto window = sys::graphics::window::make_window();
  window->
    set_title( L"ogl window" )
    .set_size( 500, 500 )
    .set_position( 500, 500 )
    .show()
    ;

  window->add_listener( event_t::close, [] ( event_data_t data )
  {
    ::std::cout << "close" << "\n";
    std::exit( EXIT_SUCCESS );
  } );

  auto ctx = sys::graphics::ogl::make_gl_context();
  ctx->init( window->get_native_handle() ).make_current();

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

  std::vector< glm::vec3 > vertices = { {0,0,0}, {1,0,0}, {0,1,0} };
  auto data = gsl::as_span( vertices );

  GLuint buffer = {};
  ::glCreateBuffers( 1, &buffer );
  ::glNamedBufferData( buffer, data.size_bytes(), data.data(), GL_STATIC_DRAW );

  GLuint vao = {};
  ::glCreateVertexArrays( 1, &vao );
  ::glEnableVertexArrayAttrib( vao, 0 );
  ::glVertexArrayVertexBuffer( vao, 0, buffer, 0, sizeof glm::vec3 );
  ::glVertexArrayAttribFormat( vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
  ::glVertexArrayAttribBinding( vao, 0, 0 );

  auto draw = [&]
  {
    GLfloat color [4] = {};
    ::glClearBufferfv( GL_COLOR, 0, color );
    ::glUseProgram( program );

    ::glBindVertexArray( vao );
    ::glDrawArrays( GL_POINTS, 0, (GLsizei)vertices.size() );

    ctx->swap_buffers();
  };

  window->add_listener( event_t::resize, [] ( event_data_t data )
  {
    ::glViewport( 0, 0, data.size.x, data.size.y );
  } );

  window->add_listener( event_t::draw, [&draw] ( event_data_t data )
  {
     draw();
  } );

  sys::graphics::window::message_loop
  (
    [&events, &program, &make_shader_program, &draw]
  {
    event e = {};
    if (events.try_pop( e ))
    {
      ::glDeleteProgram( program );
      program = make_shader_program();
      std::cout << "on_shader_change" << "\n";
      draw();
    }
  }
  );

  return 0;
}
