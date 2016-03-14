#include <gl/glew.h>
#pragma comment(lib,"opengl32.lib")
#include <SFML/Window.hpp>

#include <gsl.h>
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
  GLuint program {};

  sf::Window window { sf::VideoMode( 700, 700 ), "opengl window" };
  ::glewInit();

  sys::folder_monitor folder_monitor(
    std::tr2::sys::path( shaders_path ),
    [&] ( const std::wstring & file )
  {
    events.push( file );
  } );

  folder_monitor.start();


  auto render =
    []
  {
    //boost::timer::auto_cpu_timer timer;

    GLfloat color [] = { 0.3f, 0.f, 0.f, 0.f };
    ::glClearBufferfv( GL_COLOR, 0, color );

    ::glBegin( GL_TRIANGLES );
    ::glColor3f( 0.128f, 0.0f, 0.255f );
    ::glVertex2f( 0, 0 );
    ::glColor3f( 0.0f, 0.128f, 0.255f );
    ::glVertex2f( 1, 0 );
    ::glColor3f( 0.0f, 0.128f, 0.255f );
    ::glVertex2f( 0, 1 );
    ::glEnd();
  };

  while (window.isOpen())
  {
    std::wstring ee;

    while (events.try_pop( ee ))
    {
      ::glDeleteProgram( program );
      program = gl::create_program( gl::load_shaders( shaders_path ), &std::clog );
      //std::wcout << "events : " << ee << "\n";
      //std::cout << program << "\n";
    }

    sf::Event e {};
    while (window.pollEvent( e ))
      if (e.type == sf::Event::EventType::Closed)
        window.close();
    render(), window.display();
  }

  return 0;
}
