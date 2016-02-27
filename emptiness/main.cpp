#include <gl/glew.h>
#pragma comment(lib,"opengl32.lib")
#include <SFML/Window.hpp>

#include <iostream>

#include "folder_monitor.h"


int main( int argc, char * argv [] )
{
  //sf::Window window { sf::VideoMode( 700, 700 ), "opengl window" };
  //::glewInit();

  //auto render =
  //  []
  //{
  //  GLfloat color [] = { 0.3f, 0.f, 0.f, 0.f };
  //  ::glClearBufferfv( GL_COLOR, 0, color );

  //  ::glBegin( GL_TRIANGLES );
  //  ::glColor3f( 0.128f, 0.0f, 0.255f );
  //  ::glVertex2f( 0, 0 );
  //  ::glColor3f( 0.0f, 0.128f, 0.255f );
  //  ::glVertex2f( 1, 0 );
  //  ::glColor3f( 0.0f, 0.128f, 0.255f );
  //  ::glVertex2f( 0, 1 );
  //  ::glEnd();

  //};

  //while (window.isOpen())
  //{
  //  sf::Event e {};
  //  while (window.pollEvent( e ))
  //    if (e.type == sf::Event::EventType::Closed)
  //      window.close();
  //    else
  //      render(), window.display();
  //}


  sys::folder_monitor folder_monitor(
    L"C:/dev/",
    [] ( const std::wstring & file )
  {
    std::wcout << file << "\n";
  } );

  folder_monitor.start();

  while (true);

  return 0;
}