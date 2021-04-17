#ifndef VECTOR2_H
#define VECTOR2_H
#include <iostream>
#include "SFML/Graphics.hpp"


template<class T>
class Vector2 final
{
public:
  T x;
  T y;

Vector2();

Vector2(T x);

Vector2(T x, T y);

template<class U>
explicit Vector2<T>(const Vector2<U>& vector);


sf::Vector2<float> ConverttoSF();



Vector2<T> add( Vector2<T>);
Vector2<T> sub( Vector2<T>);
Vector2<T> divide( int);
Vector2<T> mult( int);
float dist( Vector2<T>);
Vector2<T> setMag( float);
Vector2<T> limit( int);



Vector2<T>& operator=(const Vector2<T>& Vector2Obj);

Vector2<T> operator+( const Vector2<T>& Right );

Vector2<T> operator-( const Vector2<T>& Right);


bool operator!=(const Vector2<T> &obj);

bool operator==(const Vector2<T> &obj);

};

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float> Vector2f;



#endif /* VECTOR2_H */
