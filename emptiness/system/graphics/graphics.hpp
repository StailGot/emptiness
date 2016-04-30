#pragma once

namespace sys { namespace graphics { 

  class icontex
  {
    using that = icontex;

  public:
    virtual ~icontex()             = 0 {};

    virtual that & create       () = 0;
    virtual that & create       ( void * system_window_handle ) = 0;

    virtual that & make_current () = 0;
    virtual that & destroy      () = 0;

    virtual that & flush        () = 0;
    virtual that & swap_buffers () = 0;
  };

}}