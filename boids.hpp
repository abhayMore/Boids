#pragma once

#include "vector2.hpp"

#include <SFML/Graphics.hpp>

class Boids {
  Vector2f position;
  Vector2f velocity;
  Vector2f acceleration;
  float maxSpeed, maxForce;
  sf::Texture texture;
  sf::Sprite sprite;

 public:
  Boids();
  void AvoidEdges(const int &HEIGHT, const int &WIDTH);

  template <std::size_t S>
  Vector2f align(Boids (&otherBoids)[S], int alignmentRadius);

  template <std::size_t S>
  Vector2f separation(Boids (&otherBoids)[S], int separationRadius);

  template <std::size_t S>
  Vector2f cohesion(Boids (&otherBoids)[S], int cohesionRadius);

  Vector2f collision(std::vector<sf::CircleShape> &shape);

  template <std::size_t S>
  void flock(Boids (&otherBoids)[S], std::vector<sf::CircleShape> &shape,
             int alignmentRadius, int cohesionRadius, int separationRadius);

  void update();
  void draw(sf::RenderWindow &window);

  friend bool operator==(const Boids &a, const Boids &b);
  friend bool operator!=(const Boids &a, const Boids &b);
};
