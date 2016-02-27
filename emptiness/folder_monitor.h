#pragma once

#include <atomic>
#include <string>
#include <functional>
#include <memory>


namespace sys {

  class folder_monitor
  {
  public:
    using callback_t = std::function<void( const std::wstring & )>;

  private:
    std::wstring _folder;
    void * _handles [2] = {};

    std::shared_ptr<callback_t> _callback;
    std::atomic_bool _alive = false;


  private:
    void do_work();

  public:
    folder_monitor() = default;
    ~folder_monitor();

    void start();
    void start( const std::wstring & folder );
    void set_callback( callback_t callback );
    void stop();
  };

}
