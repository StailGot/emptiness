#include "window.hpp"
#include "detail\detail.hpp"

#include <windows.h>
#include <windowsx.h>

namespace sys{ namespace graphics{ namespace window {

class window : public iwindow
{
private:
  using that = iwindow;

  HWND _hwnd;

public:

  virtual that & create() override
  {
    return _hwnd = detail::create_window( {}, {}, {}, {}, {} ), *this;
  }
  virtual that & set_size( int32_t w, int32_t h ) override
  {
    return ::SetWindowPos( _hwnd, {}, {}, {}, w, h, SWP_NOMOVE | SWP_NOZORDER ), *this;
  }
  virtual that & set_position( int32_t x, int32_t y ) override
  {
    return ::SetWindowPos( _hwnd, {}, x, y, {}, {}, SWP_NOSIZE | SWP_NOZORDER ), *this;
  }
  virtual that & show() override
  {
    return ::ShowWindow( _hwnd, SW_NORMAL ), *this;;
  }
  virtual that & hide() override
  {
    return ::ShowWindow( _hwnd, SW_HIDE ), *this;
  }

  virtual native_handle get_native_handle() override
  {
    return _hwnd;
  }

  virtual that & set_title( const std::wstring & title ) override
  {
    return ::SetWindowText( _hwnd,  title.c_str() ), *this;   
  }
};

std::unique_ptr<iwindow> make_window()
{
  return std::make_unique<window>();
}

void message_loop()
{
  return message_loop( {} );
}

void message_loop( std::function<void()> on_idle )
{
  MSG msg {};
  while (true)
  {
    if (::PeekMessageW( &msg, {}, {}, {}, TRUE ))
    {
      ::TranslateMessage( &msg );
      ::DispatchMessageW( &msg );
    }
    else
      if (on_idle)
        on_idle();
  }
}

}}} // sys::graphics::window