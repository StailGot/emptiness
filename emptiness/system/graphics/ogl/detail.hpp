#pragma once

#include <windows.h>
#include <tuple>

namespace sys { namespace graphics {  namespace ogl {

  namespace detail{
  auto create_context()
  {
    auto hdc = ::GetDC( {} );

    PIXELFORMATDESCRIPTOR pfd {};
    ::SetPixelFormat( hdc, ::ChoosePixelFormat( hdc, &pfd ), &pfd );
    
    auto ctx = ::wglCreateContext( hdc );
    return std::make_tuple( hdc, ctx );
  }} // detail

}}} // sys::graphics::ogl