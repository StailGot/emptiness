#include <iostream>
#include <iomanip>

#define EXTERN __declspec(dllexport)

const auto pi = acos( -1 );
auto on_init = [] {std::cout.precision( std::numeric_limits<long double>::digits ); return 0; }();

template <typename T> struct EXTERN A
{
  T foo();
  void print( T v );
};

template<typename T>
inline void A<T>::print( T v )
{
  std::cout << v << "\n";
}

template<typename T>
inline T A<T>::foo()
{
  return T( pi );
}

template<>
inline double A<double>::foo()
{
  return double( pi * 2 );
}
//
//template<>
//inline float A<float>::foo()
//{
//  return float( pi );
//}

template struct A<float>;
template struct A<double>;
template struct A<int>;
