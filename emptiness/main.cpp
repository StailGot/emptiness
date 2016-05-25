#include <iostream>

#include <type_traits>
#include <utility>
#include <tuple>
#include <iosfwd> 
#include <string>


template< std::size_t I, class T >
struct node
{
  T value;
  node( T v )
    : value( v )
  {
  }
};

template< class Indices, class ... T >
struct __list;

template<size_t I, class T>
decltype(auto) __get( node<I, T> & v ) noexcept
{
  return v.value;
}

template<size_t I, class T>
decltype(auto) __get( const node<I, T> & v ) noexcept
{
  return v.value;
}

template< size_t ... I, class ... T >
struct __list< std::index_sequence<I ... >, T ... >: node<I, T>...
{
  using that = __list< std::index_sequence<I ... >, T ... >;

private:
  template< size_t ... I >
  __list( T ... v, std::index_sequence< I ... > )
    : node<I, T>( v ) ...
  {
  }
public:

  template< size_t ... I >
  __list( T ... v ) : __list( v..., std::make_index_sequence< sizeof...(T)> {} )
  {
  }


  template< size_t ... I, class ... R >
  that & operator = ( const __list<std::index_sequence< I ... >, R...> & rhs ) noexcept
  {
    that & lhs = *this;
    int dummy [] = { ((__get<I>( lhs ) = __get<I>( rhs )), 0)... };
    return *this;
  }

};

template< class ... T >
struct list: __list< std::make_index_sequence<sizeof...(T)>, T ...  >
{
  using super_t = __list< std::make_index_sequence<sizeof...(T)>, T ...  >;

  using  super_t::__list;
  using  super_t::operator =;
};


template<size_t I, class ... T>
decltype(auto) get( list<T...> & v ) noexcept
{
  return __get<I>( v );
}


struct holder
{
  template< class T >
  operator T()
  {
    return {};
  }
};


template< size_t ... I, class T, class E = decltype(T { (I,holder{})... }) >
auto __try_init( T, std::index_sequence<I...> )
{
  return std::true_type {};
}

auto __try_init( ... )
{
  return std::false_type {};
}

template< size_t I, class T  >
auto try_init( T t )
{
  return  __try_init( t, std::make_index_sequence<I>{} );
}

template< size_t I, class T, class Next >
struct _member_size;

template< size_t I, class T >
struct _member_size< I, T, std::true_type >
{
  constexpr static size_t value = I;
};

template< size_t I, class T >
struct _member_size< I, T, std::false_type >
{
  constexpr static size_t value = _member_size < I - 1, T, decltype(try_init<I - 1>( T {} )) > ::value;
};

template< class T >
using member_size = _member_size< 100, T, std::false_type >;

template< size_t I >
struct h
{
  static const size_t value;
};

template<> const size_t h<10>::value = 72;

int main()
{
  struct struct_
  {
    int i;
    std::string j;
    int k;
    double e;
    float f;
  };

  ;
  std::cout << member_size< struct_ >::value << "\n";
  std::cout << typeid(try_init< member_size< struct_ >::value >( struct_ {} )).name() << "\n";

  int i = 10;
  int j = 20;
  std::string s = "sad";

  std::cout
    << i << " "
    << j << " "
    << s << "\n"
    ;

  list<int, int, std::string> l { 17, 54, "das" };
  list<int &, int &, std::string &> r { i, j, s };
  r = l;

  std::cout
    << i << " "
    << j << " "
    << s << "\n"
    ;

}


