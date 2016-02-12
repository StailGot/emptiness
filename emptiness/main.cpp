#include <gl/glew.h>
#pragma comment(lib,"opengl32.lib")
#include <SFML/Window.hpp>

#include <iostream>

int main( int argc, char * argv [] )
{
  sf::Window window { sf::VideoMode( 700, 700 ), "opengl window" };
  ::glewInit();

  auto render =
    []
  {
    GLfloat color [] = { 0.3f, 0.f, 0.f, 0.f };
    ::glClearBufferfv( GL_COLOR, 0, color );
  };

  while (window.isOpen())
  {
    sf::Event e {};
    while (window.pollEvent( e ))
    {
      if (e.type == sf::Event::EventType::Closed)
        window.close();
      else
        render(), window.display();
    }
  }

  return 0;
}