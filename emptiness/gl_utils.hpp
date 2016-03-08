#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <gl/glew.h>

namespace gl
{
  namespace detail
  {
    template<typename Fn>
    auto get_iv( GLuint obj, GLenum pname, Fn fn )
    {
      GLint result {};
      fn( obj, pname, &result );
      return result;
    }
  }


  inline auto check_for_errors( std::ostream * os )
  {
    std::string error_message;

    if (os)
      switch (::glGetError())
      {
        case GL_NO_ERROR: break;
        case GL_INVALID_ENUM: error_message = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE: error_message = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: error_message = "GL_INVALID_OPERATION"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error_message = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
        case GL_OUT_OF_MEMORY: error_message = "GL_OUT_OF_MEMORY"; break;
        case GL_STACK_UNDERFLOW: error_message = "GL_STACK_UNDERFLOW"; break;
        case  GL_STACK_OVERFLOW: error_message = "GL_STACK_OVERFLOW"; break;
      }

    if (os && !error_message.empty())
      *os << error_message << "\n";
  }


  inline  auto create_shader( const std::string & src, GLenum shader_type, std::ostream * os )
  {
    GLuint shader = ::glCreateShader( shader_type );
    auto ptr = src.c_str();
    ::glShaderSource( shader, 1, &ptr, nullptr );
    ::glCompileShader( shader );

    if (detail::get_iv( shader, GL_COMPILE_STATUS, ::glGetShaderiv ) == GL_FALSE)
    {
      if (os)
        if (GLint length = detail::get_iv( shader, GL_INFO_LOG_LENGTH, ::glGetShaderiv ))
        {
          std::vector<GLchar> info_log( length );
          ::glGetShaderInfoLog( shader, (GLsizei)std::size( info_log ), &length, std::data( info_log ) );
          *os << std::data( info_log ) << "\n";
        }
      ::glDeleteShader( shader );
      shader = {};
    }
    return shader;
  }


  inline auto create_program( std::initializer_list<GLuint> shaders, std::ostream * os )
  {
    GLuint program = ::glCreateProgram();
    for (auto && shader : shaders)
      ::glAttachShader( program, shader )
      , ::glDeleteProgram( program );

    ::glLinkProgram( program );

    if (detail::get_iv( program, GL_LINK_STATUS, ::glGetProgramiv ) == GL_FALSE)
    {
      if (os)
        if (GLint length = detail::get_iv( program, GL_INFO_LOG_LENGTH, ::glGetProgramiv ))
        {
          std::vector<GLchar> info_log( length );
          ::glGetProgramInfoLog( program, (GLsizei)std::size( info_log ), &length, std::data( info_log ) );
          *os << std::data( info_log ) << "\n";
        }
      ::glDeleteProgram( program );
      program = {};
    }
    return program;
  }

}