#include "ogl.hpp"
#include "detail.hpp"

#include <tuple>

#include <gl/gl.h>
#include <windows.h>

namespace sys { namespace graphics {  namespace ogl {

  class gl_contex: public icontex
  {
    HGLRC _hglrc = {};
    HDC   _hdc = {};

    using that = gl_contex;

  public:
    ~gl_contex() override
    {
      gl_contex::destroy();
    }

    that & make_current() override
    {
      return ::wglMakeCurrent( _hdc, _hglrc ), *this;
    }

    that & init() override
    {
      return init( {} );
    }

    that & init( void * system_window_handle ) override
    {
      return
        std::tie( _hdc, _hglrc ) = detail::create_context( system_window_handle )
        , *this;
    }

    that & destroy() override
    {
      return ::wglDeleteContext( _hglrc ), ::DeleteDC( _hdc )
        , *this;
    }

    that & flush() override
    {
      return ::glFlush(), *this;
    }

    that & swap_buffers() override
    {
      return ::SwapBuffers( _hdc ), *this;
    }
  };

  std::unique_ptr<icontex> make_gl_context()
  {
    return std::make_unique<gl_contex>();
  }

}}} // sys::graphics::ogl