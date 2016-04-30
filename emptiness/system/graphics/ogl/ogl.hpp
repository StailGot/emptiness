#pragma once

#include <memory>
#include <system\graphics\graphics.hpp>

namespace sys { namespace graphics {  namespace ogl {
  std::unique_ptr<icontex> make_gl_context();
}}}