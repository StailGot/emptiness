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

class application_t
{
private:
  using that = application_t;

public:
  enum class event: uint32_t { on_shader_change };

  using iwindow      = sys::graphics::window::iwindow;
  using icontex      = sys::graphics::icontex;

  using event_t      = sys::graphics::window::event_t;
  using event_data_t = sys::graphics::window::event_data_t;

  using callback_t   = sys::graphics::window::iwindow::callback_t;

  using path_t       = std::tr2::sys::path;

private:

  concurrency::concurrent_queue<event> _events;
  std::unique_ptr<sys::folder_monitor> _shader_folder_monitor;
                                       
  std::unique_ptr<iwindow>             _window;
  std::unique_ptr<icontex>             _conext;
                                       
  path_t                               _shader_path;
  std::vector<glm::vec3>               _vertices;

  GLuint _program {};
  GLuint _buffer {};
  GLuint _vao {};

  void init_draw_data()
  {
    _vertices = { { 0,0,1 }, { 1,0,1 }, { 0,1,1 } };

    auto data = gsl::as_span( _vertices );

    ::glCreateBuffers( 1, &_buffer );
    ::glNamedBufferData( _buffer, data.size_bytes(), data.data(), GL_STATIC_DRAW );

    ::glCreateVertexArrays( 1, &_vao );
    ::glEnableVertexArrayAttrib( _vao, 0 );
    ::glVertexArrayVertexBuffer( _vao, 0, _buffer, 0, sizeof glm::vec3 );
    ::glVertexArrayAttribFormat( _vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
    ::glVertexArrayAttribBinding( _vao, 0, 0 );

    _program = make_shader_program( _shader_path );

    ::glEnable( GL_PROGRAM_POINT_SIZE );
  }

  void init_window()
  {
    _window = sys::graphics::window::make_window();
    _window->
      set_title( L"ogl window" )
      .set_size( 500, 500 )
      .set_position( 500, 500 )
      .show()
      ;

    _conext = sys::graphics::ogl::make_gl_context();
    _conext->init( _window->get_native_handle() ).make_current();

    ::glewInit();
  }

  GLuint make_shader_program( const path_t & shaders_path )
  {
    namespace gl = sys::graphics::ogl;
    auto shaders = gl::load_shaders( shaders_path.string() );
    return gl::create_program( shaders, &std::cout );
  };

  void process_message_loop()
  {
    sys::graphics::window::message_loop( [this]
    {
      event e = {};
      if (_events.try_pop( e ))
      {
        ::glDeleteProgram( _program );
        _program = make_shader_program( _shader_path );
        std::cout << "on_shader_change" << "\n";
        draw();
      }
    } );
  }

  auto on_shader_change( const path_t & name )
  {
    _events.push( event::on_shader_change );
  };

public:

  application_t()
  {
    init_window();
  };

  that & init()
  {
    return /*init_window(),*/ init_draw_data(), *this;
  }

  that & run()
  {
    return init(), process_message_loop(), *this;
  }

  that & add_listener( event_t event, callback_t callback )
  {
    if (_window)
      _window->add_listener( event, std::move( callback ) );

    return *this;
  }

  that & set_sahders_path( const path_t & path )
  {
    auto event_handler = [this] ( auto && name ) { on_shader_change( name ); };
    _shader_folder_monitor =
      std::make_unique<sys::folder_monitor>( path, event_handler );

    _shader_folder_monitor->start();
    return _shader_path = path, *this;
  }

  that & draw()
  {
    GLfloat color [4] = {};
    ::glClearBufferfv( GL_COLOR, 0, color );
    ::glUseProgram( _program );

    ::glBindVertexArray( _vao );
    ::glDrawArrays( GL_POINTS, 0, (GLsizei)_vertices.size() );

    if (_conext)
      _conext->swap_buffers();
    return *this;
  }

};


int main( int argc, char * argv [] )
{
  ::setlocale( 0, "" );

  auto shaders_path = std::tr2::sys::path { sys::options::parse_options( argc, argv, "shaders" ) };

  application_t app;
  app
    .init()
    .add_listener( application_t::event_t::draw,   [&app] ( ... ) { app.draw(); } )
    .add_listener( application_t::event_t::close,  [&app] ( ... ) { std::exit( EXIT_SUCCESS ); } )
    .add_listener( application_t::event_t::resize, [&app] ( application_t::event_data_t data ) { ::glViewport(0,0, data.size.x, data.size.y); } )

    .set_sahders_path( shaders_path.string() )
    .run()
    ;

  return 0;
}
