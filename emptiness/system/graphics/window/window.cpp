#include "window.hpp"
#include "detail\detail.hpp"

#include <unordered_map>
#include <functional>

#include <windows.h>
#include <windowsx.h>

namespace sys{ namespace graphics{ namespace window {

class window : public iwindow
{
private:
  using that = iwindow;
  using dispatcher_t = std::unordered_map< event_t, callback_t >;

  HWND _hwnd = {};

public:

  window()
  {
    init();
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

private:
  
  virtual that & init() override
  {
    return _hwnd = detail::create_window( {}, {}, {}, {}, {}, window_proc ), *this;
  }

  static dispatcher_t & get_dispatcher()
  {
    static dispatcher_t dispatcher;
    return dispatcher;
  }

  static  LRESULT _stdcall window_proc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
  {
    using detail::lparam_t;
    using detail::wparam_t;
    using detail::message_t;


    const auto event_and_data = detail::convert_message_data( message_t{Msg}, lparam_t{lParam}, wparam_t{wParam} );
    
    if( event_and_data.event != event_t::undefined )
      if (auto && callback = get_dispatcher() [event_and_data.event])
        callback( event_and_data );

    return ::DefWindowProc( hWnd, Msg, wParam, lParam );
  }

  virtual that & add_listener( event_t event, callback_t callback ) override
  {
    return get_dispatcher()[event] = std::move( callback ), *this;
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
  while (msg.message != WM_QUIT)
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