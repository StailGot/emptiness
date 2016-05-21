#pragma once

#include <windows.h>
#include <windowsx.h>

#include <string>

#include "event_converter.hpp"

namespace sys{ namespace graphics{ namespace window {

using window_callback_t = LRESULT (_stdcall *) ( HWND, UINT, WPARAM, LPARAM );

namespace detail{
  auto create_window( std::wstring title, int32_t x, int32_t y, int32_t w, int32_t h, window_callback_t callback )
  {
    constexpr auto class_name = L"render.window.class";

    auto make_window_class =
      [&callback] ( const std::wstring & class_name )
    {
      WNDCLASSEXW wc {};
      wc.cbSize        = sizeof wc;
      wc.lpfnWndProc   = callback ? callback : ::DefWindowProcW;
      wc.lpszClassName = L"render.window.class";
      wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

      wc.hCursor       = ::LoadCursorW( {}, IDC_ARROW );
      wc.hIcon         = ::LoadIconW( {}, IDI_APPLICATION );

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
} // detail


}}} // sys::graphics::window