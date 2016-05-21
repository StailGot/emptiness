#pragma once

#include <windows.h>
#include <windowsx.h>

#include "strong_type.hpp"

namespace sys{ namespace graphics{ namespace window {

using window_callback_t = LRESULT (_stdcall *) ( HWND, UINT, WPARAM, LPARAM );

namespace detail{

  using sys::type::newtype;

  using message_t = struct : newtype<UINT>   { using newtype<UINT>::newtype; };
  using lparam_t  = struct : newtype<LPARAM> { using newtype<LPARAM>::newtype; };
  using wparam_t  = struct : newtype<WPARAM> { using newtype<WPARAM>::newtype; };

  inline auto convert_message_data( message_t msg, lparam_t lparam, wparam_t wparam )
  {
    auto get_point = []( lparam_t lparam ) -> event_data_t::point2d_t { 
      return {GET_X_LPARAM(lparam.get()), GET_Y_LPARAM(lparam.get()) }; 
    };

    using mouse_button_t = event_data_t::mouse_button_t;
    event_data_t   data  = {};
    event_t      & event = data.event;

    switch ( msg )
    {
      case WM_CLOSE     :   event = event_t::close;                    break;
      case WM_PAINT     :   event = event_t::draw;                     break;

      case WM_MOUSEMOVE :   event = event_t::mouse_move;
                            data.mouse.button = (wparam & MK_LBUTTON) ? mouse_button_t::left : mouse_button_t::undefined;

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