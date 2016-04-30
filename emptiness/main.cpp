#include "emptiness_pch.hpp"

#include "folder_monitor.hpp"
#include "options.hpp"

#include <iostream>
#include <filesystem>
#include <memory>

#include <system\graphics\ogl\ogl.hpp>


int main( int argc, char * argv [] )
{
  auto path =
    std::tr2::sys::path( sys::options::parse_options( argc, argv, "shaders" ) );

  sys::folder_monitor fm { path, [] ( auto && ) { std::cout << 123 << "\n"; } };
  fm.start();

  std::cout << sys::options::parse_options( argc, argv, "shaders" ) << "\n";

  auto ctx = sys::graphics::ogl::make_gl_context();
  ctx->create().make_current();

  auto ctx2 = sys::graphics::ogl::make_gl_context();
  ctx2->create().make_current();

  std::cout << ::glewInit() << "\n";

  std::array<uint8_t, 100> data {};
  GLuint buffer {};

  ::glGenBuffers( 1, &buffer );
  ::glBindBuffer( GL_ARRAY_BUFFER, buffer );
  ::glBufferData( GL_ARRAY_BUFFER, gsl::as_span( data ).size_bytes()
                  , data.data(), GL_STATIC_DRAW );

  auto map_buffer = []
  {
    auto data_ptr = ::glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
    std::cout << data_ptr << "\n";
    ::glUnmapBuffer( GL_ARRAY_BUFFER );
  };

  map_buffer();
  ctx->make_current();
  map_buffer();
  ctx2->make_current();
  map_buffer();

  //std::getchar();
  return 0;
}

