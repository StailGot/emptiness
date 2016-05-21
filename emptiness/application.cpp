#include "application.hpp"

#include <gsl.h>
#include <system\graphics\ogl\ogl.hpp>
#include <system\graphics\ogl\gl_utils.hpp>

namespace application{

void application_t::init_draw_data()
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

void application_t::init_window()
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

GLuint application_t::make_shader_program( const path_t & shaders_path )
{
  namespace gl = sys::graphics::ogl;
  auto shaders = gl::load_shaders( shaders_path.string() );
  return gl::create_program( shaders, &std::cout );
}

void application_t::process_message_loop()
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

auto application_t::on_shader_change( const path_t & name )
{
  _events.push( event::on_shader_change );
}

application_t::application_t()
{
  init_window();
}

application_t & application_t::init()
{
  return /*init_window(),*/ init_draw_data(), *this;
}

application_t & application_t::run()
{
  return init(), process_message_loop(), *this;
}

application_t & application_t::add_listener( event_t event, callback_t callback )
{
  if (_window)
    _window->add_listener( event, std::move( callback ) );

  return *this;
}

application_t & application_t::set_sahders_path( const path_t & path )
{
  auto event_handler = [this] ( auto && name ) { on_shader_change( name ); };
  _shader_folder_monitor =
    std::make_unique<sys::folder_monitor>( path, event_handler );

  _shader_folder_monitor->start();
  return _shader_path = path, *this;
}

application_t & application_t::draw()
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

}