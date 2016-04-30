#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <functional>

namespace sys{ namespace graphics{ namespace window {

class iwindow
{  
public:
  using that = iwindow;
  using native_handle = void *;

public:
 virtual        ~iwindow    () = 0 {};

 virtual that & create      () = 0;
 virtual that & set_size    ( int32_t w, int32_t h ) = 0;
 virtual that & set_position( int32_t x, int32_t y ) = 0;
 virtual that & set_title   ( const std::wstring & title   ) = 0;


 virtual that & show        () = 0;
 virtual that & hide        () = 0;

 virtual native_handle get_native_handle() = 0;
};

std::unique_ptr<iwindow> make_window();
void message_loop();
void message_loop( std::function<void()> on_idle ) ;


}}}