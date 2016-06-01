#include <iostream>
#include <type_traits>
#include <functional>
#include <tuple>
#include <vector>

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

int main( int argc, char * argv [] )
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

  return 0;
}