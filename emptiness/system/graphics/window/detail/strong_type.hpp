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

}}