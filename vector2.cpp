#include "vector2.hpp"

#include <cmath>

template<class T>
Vector2<T>::Vector2():x(0.0f),y(0.0f)
{
}

template<class T>
Vector2<T>::Vector2(T x): x(x), y(x)
{
}

template<class T>
Vector2<T>::Vector2(T x, T y): x(x),y(y)
{
}

template<class T>
sf::Vector2<float> Vector2<T>::ConverttoSF()
{
  return sf::Vector2<float>(static_cast<T>(x),static_cast<T>(y));
}

template<class T>
Vector2<T> Vector2<T>::add( Vector2<T> a)
{
  return Vector2<T>(this->x + a.x, this->y + a.y);
}

template<class T>
Vector2<T> Vector2<T>::sub( Vector2<T> a)
{
  return Vector2<T>(this->x - a.x, this->y - a.y);
}

template<class T>
Vector2<T> Vector2<T>::divide( int n)
{
  return Vector2<T>(this->x/n, this->y/n);
}

template<class T>
Vector2<T> Vector2<T>::mult( int n)
{
  return Vector2<T>(this->x * n, this->y * n);
}

template<class T>
float Vector2<T>::dist( Vector2<T> a)
{
  T DiffX = this->x - a.x;
  T DiffY = this->y - a.y;

  return sqrt(DiffX * DiffX + DiffY * DiffY);
}

template<class T>
Vector2<T> Vector2<T>::setMag(float NewMag)
{

  auto mag = sqrt(this->x * this->x + this->y * this->y);
  return Vector2<T>((this->x * NewMag) / mag, (this->y * NewMag) / mag);
}


template<class T>
Vector2<T> Vector2<T>::limit(int n)
{

  auto mag = sqrt(this->x * this->x + this->y * this->y);
  if(mag > n)
  {
    return Vector2<T>((this->x * n) / mag, (this->y * n) / mag);
  }
  return *this;
}

template<class T>
Vector2<T> Vector2<T>::operator+( const Vector2<T>& Right )
{
  return Vector2<T>(x + Right.x, y + Right.y);
}

template<class T>
Vector2<T> Vector2<T>::operator-( const Vector2<T>& Right)
{
  return Vector2<T>(x - Right.x, y - Right.y);
}

template<class T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& Vector2Obj)
{
  x = Vector2Obj.x;
  y = Vector2Obj.y;
  return *this;
}

template<class T>
bool Vector2<T>::operator!=(const Vector2<T> &obj)
{
  return !((x == obj.x) && (y == obj.y));
}

template<class T>
bool Vector2<T>::operator==(const Vector2<T> &obj)
{
  return ((x == obj.x) && (y == obj.y));
}

template class Vector2<int>;
template class Vector2<float>;
template class Vector2<unsigned int>;
