#pragma once

#include <system\graphics\graphics.hpp>
#include <system\graphics\window\window.hpp>
#include <folder_monitor.hpp>

#include <filesystem>

#include <gl\glew.h>
#include <glm\glm.hpp>

#include <concurrent_queue.h>


namespace application{

class application_t
{
public:
  enum class event: uint32_t { on_shader_change };

  using iwindow      = sys::graphics::window::iwindow;
  using icontex      = sys::graphics::icontex;

  using event_t      = sys::graphics::window::event_t;
  using event_data_t = sys::graphics::window::event_data_t;

  using callback_t   = sys::graphics::window::iwindow::callback_t;

  using path_t       = std::experimental::filesystem::path;

private:

  concurrency::concurrent_queue<event> _events;
  std::unique_ptr<sys::folder_monitor> _shader_folder_monitor;
                                       
  std::unique_ptr<iwindow>             _window;
  std::unique_ptr<icontex>             _conext;
                                       
  path_t                               _shader_path;
  std::vector<glm::vec3>               _vertices;

  GLuint _program {};
  GLuint _buffer {};
  GLuint _vao {};

  void      init_draw_data          ();
  void      init_window             ();
  void      process_message_loop    ();

  void      on_shader_change        ( const path_t & name );
  
  static GLuint    make_shader_program     ( const path_t & shaders_path );

public:

  application_t();
  application_t & add_listener      ( event_t event, callback_t callback );
  application_t & set_sahders_path  ( const path_t & path );
  application_t & init              ();
  application_t & run               ();
  application_t & draw              ();
  application_t & lock_mouse        ( bool lock );
};

}