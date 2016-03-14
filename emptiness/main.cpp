#include <gl/glew.h>
#pragma comment(lib,"opengl32.lib")
#include <SFML/Window.hpp>

#include <gsl.h>
#include <glm\common.hpp>
#include <glm\gtc\type_ptr.hpp>


#include <concurrent_queue.h>
#include <boost/timer/timer.hpp>
#include <filesystem>


#include "folder_monitor.hpp"
#include "gl_utils.hpp"
#include "system_utils.hpp"
#include "options.hpp"

int main( int argc, char * argv [] )
{
  concurrency::concurrent_queue<std::wstring> events;
  std::string shaders_path = options::parse_options( argc, argv, "shaders" );

  sf::Window window { sf::VideoMode( 700, 700 ), "opengl window" };
  ::glewInit();

  sys::folder_monitor folder_monitor(
    std::tr2::sys::path( shaders_path ),
    [&] ( const std::wstring & file )
  {
    events.push( file );
  } );

  folder_monitor.start();

  GLuint program = gl::create_program( gl::load_shaders( shaders_path ), &std::clog );
  glm::vec2 viewport { 700, 700 };
  glm::vec2 mouse;


  auto init =
    []
  {
    ::glEnable( GL_DEPTH_TEST );
    ::glEnable( GL_PROGRAM_POINT_SIZE );
  };

  boost::timer::auto_cpu_timer timer;

  auto render =
    [&program, &viewport, &timer, &mouse]
  {
    //boost::timer::auto_cpu_timer timer;

    GLfloat color [] = { 0.85f, 0.85f, 0.85f, 0.f };
    ::glClearBufferfv( GL_COLOR, 0, color );

    GLfloat one [] = { 1.f, 1.f, 1.f, 1.f };
    ::glClearBufferfv( GL_DEPTH, 0, one );

    ::glUseProgram( program );
    ::glUniform2fv( ::glGetUniformLocation( program, "viewport" ), 1, glm::value_ptr( viewport ) );
    ::glUniform2fv( ::glGetUniformLocation( program, "mouse" ), 1, glm::value_ptr( mouse ) );


    const double sec = 1000000000.0L;
    const float elapsed = float( double( timer.elapsed().wall ) / sec );

    ::glUniform1fv( ::glGetUniformLocation( program, "time" ), 1, &elapsed );


    ::glBegin( GL_POINTS );
    ::glVertex2f( 0, 0 );
    ::glEnd();
  };

  init();

  while (window.isOpen())
  {
    std::wstring ee;

    while (events.try_pop( ee ))
    {
      ::glDeleteProgram( program );
      program = gl::create_program( gl::load_shaders( shaders_path ), &std::clog );
    }

    sf::Event e {};
    while (window.pollEvent( e ))
      if (e.type == sf::Event::EventType::Closed)
        window.close();
      else
        if (e.type == sf::Event::EventType::Resized)
          (viewport = { e.size.width, e.size.height })
          , ::glViewport( 0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y );
        else
          if (e.type == sf::Event::EventType::MouseMoved)
            mouse = { (float) e.mouseMove.x, e.mouseMove.y };

    render(), window.display();
  }

  return 0;
}
