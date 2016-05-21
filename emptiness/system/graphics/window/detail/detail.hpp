#pragma once

#include <windows.h>
#include <windowsx.h>

#include <string>

namespace sys{ namespace type {
 
  template<typename T>
  struct newtype
  {
    T value = {};
    using type = T;

    newtype() = default;

    explicit newtype( const T value )
      : value( value )
    {
    };

    operator T()
    {
      return value;
    }

    auto & get()
    {
      return value;
    }
  };

}

namespace graphics{ namespace window {

using window_callback_t = LRESULT (_stdcall *) ( HWND, UINT, WPARAM, LPARAM );

namespace detail{
  auto create_window( std::wstring title, int32_t x, int32_t y, int32_t w, int32_t h, window_callback_t callback )
  {
    constexpr auto class_name = L"render.window.class";

    auto make_window_class =
      [&callback] ( const std::wstring & class_name )
    {
      WNDCLASSEXW wc {};
      wc.cbSize = sizeof wc;
      wc.lpfnWndProc = callback ? callback : ::DefWindowProcW;
      wc.lpszClassName = L"render.window.class";
      wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

      wc.hCursor = ::LoadCursorW( {}, IDC_ARROW );
      wc.hIcon = ::LoadIconW( {}, IDI_APPLICATION );

      ::RegisterClassExW( &wc );

      return wc;
    };

    static auto window_class = make_window_class( class_name );

    auto hwnd =
      ::CreateWindowExW( {}, class_name, title.c_str()
                         , /*WS_VISIBLE |*/ WS_OVERLAPPEDWINDOW
                         , x, y, w, h, {}, {}, {}, {} );
    return hwnd;
  }


  using sys::type::newtype;

  using message_t = UINT;
  using lparam_t  = struct : newtype<LPARAM> { using newtype<LPARAM>::newtype; };
  using wparam_t  = struct : newtype<WPARAM> { using newtype<WPARAM>::newtype; };

  auto convert_message_data( message_t msg, lparam_t lparam, wparam_t wparam )
  {
    auto get_point = []( lparam_t lparam ) -> event_data_t::point2d_t { 
      return {GET_X_LPARAM(lparam.get()), GET_Y_LPARAM(lparam.get()) }; 
    };

    event_data_t   data  = {};
    event_t      & event = data.event;

    switch ( msg )
    {
      case WM_CLOSE     :   event = event_t::close;                    break;
      case WM_PAINT     :   event = event_t::draw;                     break;      

      case WM_MOUSEMOVE :   event = event_t::mouse_move;
                            data.mouse.position = get_point( lparam ); break;
 
      case WM_LBUTTONUP :   event = event_t::mouse_click;
                            data.mouse.position = get_point( lparam ); break;

      case WM_LBUTTONDOWN : event = event_t::mouse_down;
                            data.mouse.position = get_point( lparam ); break;

      case WM_SIZE      :   event = event_t::resize;
                            data.size = get_point( lparam ); break;
    }
    return data;
  }

} // detail


}}} // sys::graphics::window