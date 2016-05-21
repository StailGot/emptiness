#pragma once

#include <cstdint>

namespace sys{ namespace graphics{ namespace window { namespace event {

enum class event_t : uint32_t
{
  undefined
  , close
  , mouse_move
  , mouse_click
  , resize
  , draw
};

union event_data_t
{
  using point2d_t = struct { int32_t x, y; };
  enum class mouse_button_t : uint8_t { middle, left, right };

  struct { point2d_t position; mouse_button_t button; } mouse;
  point2d_t size;
};

}}}}