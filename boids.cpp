#include "boids.hpp"

#include <cmath>

int ReturnIntRandom(int lower, int upper) {
  return (rand() % (upper - lower + 1)) + lower;
}

Boids::Boids() {
  texture.loadFromFile("arrow.png");
  sprite.setTexture(texture);
  sprite.setOrigin(
      sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));

  sprite.setScale(0.02f, 0.02f);
  position = Vector2f(ReturnIntRandom(0, 800), ReturnIntRandom(0, 500));
  sprite.setPosition(position.x, position.y);
  auto phi = rand() / (double)(RAND_MAX);
  phi *= 2.0 * M_PI;
  velocity = Vector2f(sin(phi), cos(phi));

  velocity = velocity.setMag(ReturnIntRandom(2, 4));
  acceleration = Vector2f(0.0f, 0.0f);
  maxForce = 1.5f;
  maxSpeed = 5.0f;
}

void Boids::AvoidEdges(const int &HEIGHT, const int &WIDTH) {
  if (position.x > WIDTH) {
    position.x = 0;
  } else if (position.x < 0) {
    position.x = WIDTH;
  }
  if (position.y > HEIGHT) {
    position.y = 0;
  } else if (position.y < 0) {
    position.y = HEIGHT;
  }
}

template <typename std::size_t S>
Vector2f Boids::align(Boids (&otherBoids)[S], int alignmentRadius) {
  int perceptionRadius = alignmentRadius;
  Vector2f steering = Vector2f(0.f, 0.f);
  size_t total = 0;

  for (size_t i = 0; i < sizeof(otherBoids) / sizeof(otherBoids[0]); i++) {
    auto distance = position.distance(otherBoids[i].position);
    if (*this != otherBoids[i] && distance < perceptionRadius) {
      steering = steering + otherBoids[i].velocity;
      total++;
    }
  }
  if (total > 0) {
    steering = steering / total;
    steering = steering.setMag(maxSpeed);
    steering = steering - velocity;
    steering = steering.limit(maxForce);
  }
  return steering;
}

template <typename std::size_t S>
Vector2f Boids::separation(Boids (&otherBoids)[S], int separationRadius) {
  int perceptionRadius = separationRadius;
  Vector2f steering = Vector2f(0.f, 0.f);

  size_t total = 0;
  for (size_t i = 0; i < sizeof(otherBoids) / sizeof(otherBoids[0]); i++) {
    float distance = position.distance(otherBoids[i].position);
    if ((*this != otherBoids[i]) && (distance < perceptionRadius) &&
        (distance > 0.0f)) {
      Vector2f diff = position - otherBoids[i].position;
      if (distance < 1.0f) diff = diff * (1 / (distance * distance));
      steering = steering +diff;
      total++;
    }
  }
  if (total > 0) {
    steering = steering / total;
    steering = steering.setMag(maxSpeed);
    steering = steering - velocity;
    steering = steering.limit(maxForce);
  }
  return steering;
}

Vector2f Boids::collision(std::vector<sf::CircleShape> &shape) {
  int perceptionRadius = 50;
  Vector2f steering = Vector2f(0.f, 0.f);

  size_t total = 0;
  for (size_t i = 0; i < shape.size(); i++) {
    float distance = position.distance(
        Vector2f(shape[i].getPosition().x, shape[i].getPosition().y));
    if ((distance < perceptionRadius) && (distance > 0.0f)) {
      Vector2f diff = position - Vector2f(shape[i].getPosition().x, shape[i].getPosition().y);
      if (distance < 1.0f) diff *= (1 / (distance * distance));
      steering = steering +diff;
      total++;
    }
  }
  if (total > 0) {
    steering = steering / total;
    steering = steering.setMag(maxSpeed);
    steering = steering - velocity;
    steering = steering.limit(maxForce);
  }
  return steering;
}

template <typename std::size_t S>
Vector2f Boids::cohesion(Boids (&otherBoids)[S], int cohesionRadius) {
  int perceptionRadius = cohesionRadius;
  Vector2f steering = Vector2f(0.f, 0.f);
  size_t total = 0;
  for (size_t i = 0; i < sizeof(otherBoids) / sizeof(otherBoids[0]); i++) {
    auto distance = position.distance(otherBoids[i].position);
    if (*this != otherBoids[i] && distance < perceptionRadius) {
      steering += otherBoids[i].position;
      total++;
    }
  }
  if (total > 0) {
    steering /= total;
    steering -= position;
    steering = steering.setMag(maxSpeed);
    steering -= velocity;
    steering = steering.limit(maxForce);
  }
  return steering;
}

template <std::size_t S>
void Boids::flock(Boids (&otherBoids)[S], std::vector<sf::CircleShape> &shape,
                  int alignmentRadius, int cohesionRadius,
                  int separationRadius) {
  acceleration += align(otherBoids, alignmentRadius);
  acceleration += cohesion(otherBoids, cohesionRadius);
  acceleration += separation(otherBoids, separationRadius);
  acceleration += collision(shape);
}

void Boids::update() {
  position += velocity;
  velocity += acceleration;
  velocity = velocity.limit(maxSpeed);
  acceleration *= 0;

  float angle = atan2(velocity.y, velocity.x);
  angle = angle * (180 / M_PI) - 90;
  angle = (velocity.x < 0.0f || velocity.y < 0.0f) ? angle - 180 : angle + 180;

  sprite.setRotation(angle);
  sprite.setPosition(position.x, position.y);
}

void Boids::draw(sf::RenderWindow &window) { window.draw(sprite); }

bool operator==(const Boids &a, const Boids &b) {
  return a.position == b.position &&
         a.velocity == b.velocity &&
         a.acceleration == b.acceleration;
}

bool operator!=(const Boids &a, const Boids &b) { return !(a == b); }

template void Boids::flock(Boids (&otherBoids)[200],
                           std::vector<sf::CircleShape> &shape,
                           int alignmentRadius, int cohesionRadius,
                           int separationRadius);
