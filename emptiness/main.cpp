
class private_t
{
  int foo()
  {
    return 42;
  }
  int foo( char * )
  {
    return 13;
  }
  void foo( char *, int )
  {
  }

  int bar = 100500;
  void * mo;
};

namespace utils
{
  inline namespace access
  {
    template<class T>
    struct private_ptr_t
    {
      using type = typename T::type;
      static type value;
    };

    template<class T>
    typename private_ptr_t<T>::type private_ptr_t<T>::value;

    template<class T, typename T::type ptr>
    struct private_ptr_init_t
    {
      struct _
      {
        _()
        {
          private_ptr_t<T>::value = ptr;
        }
      };
      static _ __;
    };

    template< class T, typename T::type ptr>
    typename private_ptr_init_t<T, ptr>::_ private_ptr_init_t<T, ptr>::__;
  }
}

struct T1
{
  using type = int( private_t::* );
};
template struct utils::private_ptr_init_t<T1, &private_t::bar>;

int main( int argc, char * argv [] )
{
  private_t p;
  int i = p.*utils::access::private_ptr_t<T1>::value;

  return 0;
}