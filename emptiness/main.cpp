#include <gl/glew.h>
#pragma comment(lib,"opengl32.lib")
#include <SFML/Window.hpp>

#include <iostream>
#include <concurrent_queue.h>
#include <boost/timer/timer.hpp>

//#include "folder_monitor.h"


int main( int argc, char * argv [] )
{
  concurrency::concurrent_queue<std::wstring> events;

  //sys::folder_monitor folder_monitor(
  //  L"C:/dev/",
  //  [&] ( const std::wstring & file )
  //{
  //  events.push( file );
  //} );

  //folder_monitor.start();


  sf::Window window { sf::VideoMode( 700, 700 ), "opengl window" };
  ::glewInit();

  auto render =
    []
  {
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
      std::wcout << "events : " << ee << "\n";

    sf::Event e {};
    while (window.pollEvent( e ))
      if (e.type == sf::Event::EventType::Closed)
        window.close(), exit( EXIT_SUCCESS );
      else
        render(), window.display();
  }

  return 0;
}