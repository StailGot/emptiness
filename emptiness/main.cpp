#include <iostream>
#include <type_traits>
#include <functional>
#include <tuple>
#include <vector>
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>
#include <algorithm>


namespace functional
{
  // carrying
  template<template <typename...> class Fun, typename ...Args>
  struct carry
  {
    template<typename ... Rest>
    using type = Fun< Args ..., Rest... >;
  };

  namespace detail
  {
    // map
    template<typename Fun, typename Args>
    struct __map;

    template<typename Fun, template <typename...> class List, typename ... Args >
    struct __map< Fun, List<Args...> >
    {
      using type = List < typename Fun:: template type<Args> ... >;
    };

    // reduce
    template<typename Fun, typename Args>
    struct __reduce;

    template<typename Fun, template <typename...> class List, typename ... Args >
    struct __reduce< Fun, List<Args...> >
    {
      using type = typename Fun:: template type < Args ... >;
    };
  }

  // map
  template<typename Fun, typename Args>
  using map_t = typename detail::__map< Fun, Args >::type;

  // reduce
  template<typename Fun, typename Args>
  using reduce_t = typename detail::__reduce< Fun, Args >::type;
}

namespace predicate
{
  namespace fn = functional;

  template<typename T, typename Args>
  using any_of = fn::reduce_t< fn::carry<std::disjunction>, fn::map_t< fn::carry< std::is_same, T>, Args> >;

  template<typename T, typename Args>
  using all_of = fn::reduce_t< fn::carry<std::conjunction>, fn::map_t< fn::carry< std::is_same, T>, Args> >;

  template<typename T, typename Args>
  constexpr auto any_of_v = any_of< T, Args >::value;

  template<typename T, typename Args>
  constexpr auto all_of_v = all_of< T, Args >::value;


namespace
{
  template< class Fn >
  struct _args;

  template< class Fn >
  struct _args
  {
    using type = typename _args< decltype(&Fn::operator()) >::type;
  };

  template< class Ret, class ... Args >
  struct _args< Ret( *) (Args...) >
  {
    using type = Ret( *)(Args...);
  };

  template< class C, class Ret, class ... Args >
  struct _args< Ret( C::* ) (Args...) const >
  {
    using type = Ret( *)(Args...);
  };

  template< class C, class Ret, class ... Args >
  struct _args< Ret( C::* ) (Args...) >
  {
    using type = Ret( *)(Args...);
  };
  
  template<class Fn>
  auto get_args( Fn && )
  {
    return _args<std::decay_t<Fn>>{};
  }
}


}

namespace containers
{
  template<typename ...>
  struct list_t;
}



namespace ftl
{
namespace detail {
  template< typename ... Fns, typename ...Args, size_t ...I, typename Fn>
  auto _zip_with( std::tuple<Fns...> fns, std::tuple<Args...> arg, Fn fn, std::index_sequence<I...> )
  {
    return std::make_tuple( fn( std::get<I>( fns ), std::get<I>( arg ) )... );
  }
}

template< typename FnTuple, typename ArgsTuple, typename Fn>
auto zip_with( FnTuple fns, ArgsTuple arg, Fn fn )
{
  constexpr auto size = (std::min)(std::tuple_size<FnTuple>::value, std::tuple_size<ArgsTuple>::value);
  return detail::_zip_with( fns, arg, fn, std::make_index_sequence<size>{} );
}

namespace detail {
  template<typename Fn, typename ...Args, size_t ...I>
  auto _apply( Fn fn, std::tuple<Args...> args, std::index_sequence<I...> )
  {
    return std::invoke( fn, std::forward<Args>( std::get<I>( args ))...);
  }
}

template<typename Fn, typename ...Args>
auto apply( Fn fn, std::tuple<Args...> args )
{
  return detail::_apply( fn, args
                         , std::make_index_sequence<std::tuple_size< std::tuple<Args...>>::value>{} );
}

template<typename Fn, typename Arg>
auto apply( Fn fn, Arg arg )
{
  return std::invoke( fn, arg );
}
}


int main( int argc, char * argv [] )
{
  {
    auto fun =
      [] ( int i, int y )
    {
      std::cout << i << " " << y << "\n";
      return 0;
    };

    auto fn = [] ( int i )
    {
      std::cout << i << "\n";
      return 0;
    };
    auto args = std::make_tuple( std::make_tuple( 10, 50 ), 10, std::make_tuple( 10, 50 ) );
    auto funs = std::make_tuple( fun, fn, fun );

    ftl::zip_with( funs, args, [] ( auto && ... args ) { return ftl::apply( args... ); } );
  }

  {
    //tests
    using namespace predicate;
    using containers::list_t;
    std::cout << std::boolalpha;

    static_assert(any_of_v<int, std::tuple<char, void, int>>, "any_of fail");
    static_assert(all_of_v<int, std::tuple<int, int, int>>, "all_of fail");
    static_assert(all_of_v<int, std::tuple<int, char, int>> == false, "all_of fail");
    static_assert(any_of_v<double, std::tuple<char, void, int>> == false, "any_of fail");
    static_assert(any_of_v<double, std::tuple<>> == false, "any_of fail");
    static_assert(all_of_v<int, std::tuple<int, int, int>>, "all_of fail");
    static_assert(all_of_v<int, std::tuple<char>> == false, "all_of fail");
    static_assert(all_of_v<void, std::tuple<void>>, "all_of fail");
    static_assert(any_of_v<double, std::tuple<double>>, "any_of fail");
    static_assert(any_of_v<double, std::tuple<void, void, void, void, double>>, "any_of fail");
    static_assert(any_of_v<double, std::tuple<double, void, double, void, double>>, "any_of fail");

    static_assert(any_of_v<double, list_t<void, void, void, void, double>>, "any_of fail");
    static_assert(any_of_v<double, list_t<double>>, "any_of fail");
    static_assert(any_of_v<double, list_t<char, void, int>> == false, "any_of fail");
    static_assert(any_of_v<int, list_t<char, void, int>>, "any_of fail");

    std::cout << any_of_v<int, std::tuple<char, void, int>> << "\n"; // true
    std::cout << all_of_v<int, list_t<int, int, int>> << "\n"; // true
  }

  return 0;
}
