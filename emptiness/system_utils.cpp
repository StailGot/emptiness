#include <windows.h>
#include <string>

namespace system_utils
{
  const std::string load_from_resource( const std::string & name )
  {
    auto module = ::GetModuleHandle( NULL );
    auto rc = ::FindResourceA( module, name.c_str(), "TEXT" );
    auto rc_data = ::LoadResource( module, rc );
    auto size = ::SizeofResource( module, rc );
    const char * data = static_cast<const char*>(::LockResource( rc_data ));
    return { data, size };
  }
}