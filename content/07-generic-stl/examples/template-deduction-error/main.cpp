template< typename T >
T max( T a, T b ) { return a > b ? a : b; }

int main() {
  return static_cast<int>( max( 1, 2.5 ) );   // T = int or T = double?
}
