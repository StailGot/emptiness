#include "folder_monitor.h"

#include <thread>
#include <chrono>
#include <filesystem>
#include <windows.h>

namespace sys
{
  folder_monitor::~folder_monitor()
  {
    stop();
  }


  void folder_monitor::start()
  {
    if (_alive == false)
      std::thread( [this]
    {
      _alive = true;
      _handles [0] = ::CreateEventEx( nullptr, L"", 0, EVENT_ALL_ACCESS );
      _handles [1] = ::FindFirstChangeNotificationW( _folder.c_str(), true, FILE_NOTIFY_CHANGE_LAST_WRITE );
      do_work();
      _alive = false;
    } ).detach();
  }


  void folder_monitor::start( const std::wstring & folder )
  {
    _folder = folder;
    start();
  }


  void folder_monitor::set_callback( callback_t callback )
  {
    _callback = std::make_shared<callback_t>( std::move( callback ) );
  }


  void folder_monitor::stop()
  {
    while (_alive)
      ::SetEvent( _handles [0] );

    if (_alive)
      for (auto && handle : _handles)
        ::CloseHandle( handle );
  }


  void folder_monitor::do_work()
  {
    bool stop = false;
    using namespace std::chrono;

    auto past = high_resolution_clock::now();

    while (_alive && stop == false)
    {
      auto wait_result = ::WaitForMultipleObjects( (DWORD)std::size( _handles ), _handles, FALSE, INFINITE );

      switch (wait_result)
      {
        case WAIT_OBJECT_0:
        {
          stop = true;
          std::cout << "stop event" << "\n";
          break;
        }
        case WAIT_OBJECT_0 + 1:
        {

          auto now = high_resolution_clock::now();
          auto elapsed = duration_cast<duration<double>>(now - past).count();
          past = now;

          if (elapsed > 0.01)
          {
            std::cout << "event" << "\n";
            std::cout << elapsed << "\n";

            uint8_t buffer [2048] = {};
            DWORD n_bytes = 0;
            ::ReadDirectoryChangesW( _handles [1], buffer, (DWORD)std::size( buffer ), FALSE, FILE_NOTIFY_CHANGE_LAST_WRITE, &n_bytes, nullptr, nullptr );
            FILE_NOTIFY_INFORMATION & info = *(FILE_NOTIFY_INFORMATION*)&buffer;

            if (_callback && *_callback)
              (*_callback)(info.FileName);
          }

          ::FindNextChangeNotification( _handles [1] );

          break;
        }
        case WAIT_ABANDONED:
        case WAIT_FAILED:
        case WAIT_TIMEOUT:
          stop = true;
          std::cout << "wait fail" << "\n";
          break;
        default:
          break;
      }
    }
  }

}