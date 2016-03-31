#include <iostream>
#include <type_traits>
#include <functional>
#include <tuple>
#include <vector>


template<template <typename...> class Fun, typename ...Args>
struct carry
{
  template<typename ... Rest>
  using type = Fun< Args ..., Rest... >;
};

template<typename Fun, typename Args>
struct map;

template<typename Fun, template <typename...> class List, typename ... Args >
struct map< Fun, List<Args...> >
{
  using type = std::tuple < typename Fun:: template type<Args> ... >;
};

template<typename Fun, typename Args>
using map_t = typename map< Fun, Args >::type;

template<typename Fun, typename Args>
struct reduce;

template<typename Fun, template <typename...> class List, typename ... Args >
struct reduce< Fun, List<Args...> >
{
  using type = typename Fun:: template type < Args ... >;
};

template<typename Fun, typename Args>
using reduce_t = typename reduce< Fun, Args >::type;

template<typename T, typename Args>
using any_of = reduce_t< carry<std::disjunction>, map_t< carry< std::is_same, T>, Args> >;

template<typename T, typename Args>
using all_of = reduce_t< carry<std::conjunction>, map_t< carry< std::is_same, T>, Args> >;


int main( int argc, char * argv [] )
{
  std::cout << std::boolalpha;

  std::cout << any_of < int, std::tuple<char, void, int >> ::value << "\n";
  std::cout << all_of < int, std::tuple<int, int, int >> ::value << "\n";

  return 0;
}