#include <iostream>

#define EXTERN __declspec(dllexport)

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

//template<typename T>
//inline T A<T>::foo()
//{
//  return 42;
//}

template<>
inline double A<double>::foo()
{
  return 42.5;
}

template<>
inline float A<float>::foo()
{
  return 43.7f;
}

template struct A<float>;
template struct A<double>;
template struct A<int>;
