#pragma once

#include <windows.h>
#include <tuple>

namespace sys { namespace graphics {  namespace ogl {

  namespace detail{
  auto create_context( void * system_window_handle )
  {
    auto hdc = ::GetDC( (HWND)system_window_handle );

    PIXELFORMATDESCRIPTOR pfd {};
    ::SetPixelFormat( hdc, ::ChoosePixelFormat( hdc, &pfd ), &pfd );
    
    auto ctx = ::wglCreateContext( hdc );
    return std::make_tuple( hdc, ctx );
  }} // detail

}}} // sys::graphics::ogl