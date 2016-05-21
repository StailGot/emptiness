#pragma once

#include <cstdint>

namespace sys{ namespace graphics{ namespace window { namespace event {

enum class event_t : uint32_t
{
  undefined
  , close
  , mouse_move
  , mouse_click
  , mouse_down
  , resize
  , draw
};

struct event_data_t
{
  enum class mouse_button_t: uint8_t
  {
    undefined, middle, left, right
  };
  using point2d_t = struct
  {
    int32_t x, y;
  };

  event_t event;

  union
  {
    struct
    {
      point2d_t position; mouse_button_t button;
    } mouse;

    point2d_t size;
  };
};

}}}}