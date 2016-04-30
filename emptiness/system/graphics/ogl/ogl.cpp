#include "ogl.hpp"
#include "detail.hpp"

#include <tuple>

#include <gl/gl.h>
#include <windows.h>

namespace sys { namespace graphics {  namespace ogl {

  class gl_contex : public icontex
  {
    HGLRC _hglrc = {};
    HDC   _hdc   = {};

    using that = gl_contex;

  public:
    ~gl_contex() override;

    that & make_current () override;
    that & create       () override;
    that & destroy      () override;
    that & flush        () override;
    that & swap_buffers () override;
  };

  inline gl_contex::~gl_contex()
  {
    destroy();
  }

  gl_contex::that & gl_contex::make_current()
  {
    return ::wglMakeCurrent( _hdc, _hglrc ), *this;
  }

  gl_contex::that & gl_contex::create()
  {
    return 
      std::tie(_hdc, _hglrc) = detail::create_context()
      , *this;
  }

  gl_contex::that & gl_contex::destroy()
  {
    return ::wglDeleteContext( _hglrc ), ::DeleteDC( _hdc )
      , *this;
  }

  gl_contex::that & gl_contex::flush()
  {
    return ::glFlush(), *this;
  }

  gl_contex::that & gl_contex::swap_buffers()
  {
    return ::SwapBuffers( _hdc ), *this;
  }


  std::unique_ptr<icontex> make_gl_context()
  {
    return std::make_unique<gl_contex>();
  }

}}} // sys::graphics::ogl