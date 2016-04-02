#define EXTERN __declspec(dllimport)
//#define EXTERN

template <typename T> struct EXTERN A
{
  T foo();
  void print( T v );
};

int main( int argc, char * argv [] )
{
  A<int> a;
  A<double> d;
  A<float> f;

  a.print( a.foo() );
  d.print( d.foo() );
  f.print( f.foo() );

  return 0;
}