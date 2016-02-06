
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

#define DECLARE_PRIVATE_PTR( class, member, signature )                     \
struct class##_##member { using type = signature; };                        \
template struct utils::private_ptr_init_t<class##_##member, &class::member>;

#define CALL_PRIVATE_PTR( instance, class, member )                         \
((instance).*utils::access::private_ptr_t<class##_##member>::value)


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


DECLARE_PRIVATE_PTR( private_t, bar, int( private_t::* ) )
DECLARE_PRIVATE_PTR( private_t, mo, void *(private_t::*) )
//DECLARE_PRIVATE_PTR( private_t, foo, int (private_t::*)(char *) )

int main( int argc, char * argv [] )
{
  private_t p;
  int i = CALL_PRIVATE_PTR( p, private_t, bar );

  return 0;
}