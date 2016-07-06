#include <iostream>
#include <type_traits>
#include <tuple>
#include <typeinfo>


template < typename T >
struct __args;

template < typename C, typename Ret, typename ...Args >
struct __args< Ret (C::*)(Args...) const >
{
  using type = std::tuple< Args ... >;
};

template < typename C, typename Ret, typename ...Args >
struct __args< Ret (C::*)(Args...) >
{
  using type = std::tuple< Args ... >;
};

template < typename Ret, typename ...Args >
struct __args< Ret (*)(Args...) >
{
  using type = std::tuple< Args ... >;
};


template < typename T >
struct args
{
  using type = typename __args < T >::type;
};

template < typename Fn >
auto domain( Fn f ) 
-> std::enable_if_t < 
    std::is_class<Fn>::value
  , args<decltype(&Fn::operator())> >
{
  return {};
};

template < typename Fn >
auto domain( Fn f ) 
-> std::enable_if_t < 
    !std::is_class< Fn >::value
  , args<Fn> >
{
  return {};
};


template < typename T, typename F, typename ... Fn >
auto __case_of( std::true_type, T && v, F && f, Fn && ... cases )
{
  f(v);
};

template < typename T, typename F >
auto __case_of( std::true_type, T && v, F && f )
{
  f(v);
};

template < typename T  >
auto __case_of( std::true_type, T && v )
{
    //  f(v);
};

template < typename T >
auto __case_of( std::false_type, T v )
{
  //__case_of( std::true_type{}, v, cases... );
};

template < typename T, typename F, typename ... Fn >
auto __case_of( std::false_type, T v, F f, Fn && ... cases ) 
-> std::enable_if_t < 
     std::is_same< std::tuple_element_t<0, 
        typename decltype(domain(f))::type
     >, T>::value == true
>
{
  __case_of( std::true_type{}, v, f, cases... );
};


template < typename T, typename F, typename ... Fn >
auto __case_of( std::false_type, T v, F f, Fn && ... cases ) 
-> std::enable_if_t < 
     std::is_same< std::tuple_element_t<0, 
        typename decltype(domain(f))::type
     >, T>::value == false
>
{
  __case_of( std::false_type{}, v, cases... );
};


template < typename T, typename ... Fn >
auto case_of( T v, Fn && ... cases )
{
  __case_of( std::false_type{}, v, cases... );
}

void by_float2 ( float i ) { std::cout << i << "\n"; };

int main()
{
  
  auto by_int    = [] ( int i )    { std::cout << __func__ << "int"   << i << "\n"; };
  auto by_double = [] ( double i ) { std::cout << __func__ << "doube" << i << "\n"; };
  auto by_float  = [] ( float i )  { std::cout << __func__ << "float" << i << "\n"; };

  float f = 42.5f;
  case_of( f, by_int, by_double, by_float );
  
  double d = 42.5;
  case_of( d, by_int, by_double, by_float );
  
  int i = 42;
  case_of( i, by_int, by_double, by_float );
  
  
  

  std::cout << typeid( decltype(domain( by_int ))::type ).name() << "\n";
  std::cout << typeid( decltype(domain( by_float2 ))::type ).name() << "\n";


  return 0;
}