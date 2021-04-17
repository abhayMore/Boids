#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "Vector2.h"
#include <iostream>
#include <cmath>
using namespace std;
#define WIDTH 900
#define HEIGHT 600
#define PI 3.14159265

int ReturnIntRandom(int lower, int upper)
{
  return (rand() % (upper - lower + 1)) + lower;
}

float ReturnFloatRandom(float lower, float upper)
{
  float r3 = lower + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (upper - lower)));
}

class Boid
{
  Vector2f position;
  Vector2f velocity;
  Vector2f acceleration;
  float maxSpeed, maxForce;
  sf::Texture texture;
  sf::Sprite sprite;

public:
  Boid()
  {
    texture.loadFromFile("arrow.png");
    sprite.setTexture(texture);
    sprite.setOrigin(sf::Vector2f(290.0f, 365.0f));

    sprite.setScale(0.02f, 0.02f);
    position = Vector2f(ReturnIntRandom(0, WIDTH), ReturnIntRandom(0, HEIGHT));
    sprite.setPosition(position.ConverttoSF());
    auto pi = 4.0 * atan(1.0);
    auto phi = rand() / (double)(RAND_MAX);
    phi *= 2.0 * pi;
    velocity = Vector2f(sin(phi), cos(phi));

    velocity = velocity.setMag(ReturnIntRandom(2, 4));
    acceleration = Vector2f(0.0f, 0.0f);
    maxForce = 1.5f;
    maxSpeed = 5.0f;
  }

  void AvoidEdges()
  {
    if (position.x > WIDTH)
    {
      position.x = 0;
    }
    else if (position.x < 0)
    {
      position.x = WIDTH;
    }
    if (position.y > HEIGHT)
    {
      position.y = 0;
    }
    else if (position.y < 0)
    {
      position.y = HEIGHT;
    }
  }

  template <std::size_t S>
  Vector2f align(Boid (&boids)[S], int AlignmentRadius)
  {
    int perceptionRadius = AlignmentRadius;
    Vector2f steering = Vector2f(0.f, 0.f);
    int total = 0;

    for (int i = 0; i < sizeof(boids) / sizeof(boids[0]); i++)
    {
      auto distance = position.dist(boids[i].position);
      if (*this != boids[i] && distance < perceptionRadius)
      {
        steering = steering.add(boids[i].velocity);
        total++;
      }
    }
    if (total > 0)
    {
      steering = steering.divide(total);
      steering = steering.setMag(maxSpeed);
      steering = steering.sub(velocity);
      steering = steering.limit(maxForce);
    }
    return steering;
  }

  template <std::size_t S>
  Vector2f separation(Boid (&boids)[S], int SeparationRadius)
  {
    int perceptionRadius = SeparationRadius;
    Vector2f steering = Vector2f(0.f, 0.f);

    int total = 0;
    for (int i = 0; i < sizeof(boids) / sizeof(boids[0]); i++)
    {
      float distance = position.dist(boids[i].position);
      if ((*this != boids[i]) && (distance < perceptionRadius) && (distance > 0.0f))
      {

        Vector2f diff = position.sub(boids[i].position);
        if (distance < 1.0f)
          diff = diff.mult(1 / (distance * distance));
        steering = steering.add(diff);
        total++;
      }
    }
    if (total > 0)
    {
      steering = steering.divide(total);
      steering = steering.setMag(maxSpeed);
      steering = steering.sub(velocity);
      steering = steering.limit(maxForce);
    }
    return steering;
  }
  Vector2f collision(std::vector<sf::CircleShape> &shape)
  {
    int perceptionRadius = 50;
    Vector2f steering = Vector2f(0.f, 0.f);

    int total = 0;
    for (int i = 0; i < shape.size(); i++)
    {
      float distance = position.dist(Vector2f(shape[i].getPosition().x, shape[i].getPosition().y));
      if ((distance < perceptionRadius) && (distance > 0.0f))
      {

        Vector2f diff = position.sub(Vector2f(shape[i].getPosition().x, shape[i].getPosition().y));
        if (distance < 1.0f)
          diff = diff.mult(1 / (distance * distance));
        steering = steering.add(diff);
        total++;
      }
    }
    if (total > 0)
    {
      steering = steering.divide(total);
      steering = steering.setMag(maxSpeed);
      steering = steering.sub(velocity);
      steering = steering.limit(maxForce);
    }
    return steering;
  }

  template <std::size_t S>
  Vector2f cohesion(Boid (&boids)[S], int CohesionRadius)
  {
    int perceptionRadius = CohesionRadius;
    Vector2f steering = Vector2f(0.f, 0.f);
    int total = 0;
    for (int i = 0; i < sizeof(boids) / sizeof(boids[0]); i++)
    {
      auto distance = position.dist(boids[i].position);
      if (*this != boids[i] && distance < perceptionRadius)
      {
        steering = steering.add(boids[i].position);
        total++;
      }
    }
    if (total > 0)
    {
      steering = steering.divide(total);
      steering = steering.sub(position);
      steering = steering.setMag(maxSpeed);
      steering = steering.sub(velocity);
      steering = steering.limit(maxForce);
    }
    return steering;
  }

  template <std::size_t S>
  void flock(Boid (&boids)[S], std::vector<sf::CircleShape> &shape, int AlignmentRadius, int CohesionRadius, int SeparationRadius)
  {
    Vector2f alignment = align(boids, AlignmentRadius);
    Vector2f cohes = cohesion(boids, CohesionRadius);
    Vector2f sep = separation(boids, SeparationRadius);
    Vector2f col = collision(shape);

    acceleration = acceleration.add(alignment);
    acceleration = acceleration.add(cohes);
    acceleration = acceleration.add(sep);
    acceleration = acceleration.add(col);
  }

  void update()
  {
    position = position.add(velocity);
    velocity = velocity.add(acceleration);
    velocity = velocity.limit(maxSpeed);
    acceleration = acceleration.mult(0);

    float angle = atan2(velocity.y, velocity.x);
    angle = angle * (180 / PI) - 90;
    angle = (velocity.x < 0.0f || velocity.y < 0.0f) ? angle - 180 : angle + 180;

    sprite.setRotation(angle);
    sprite.setPosition(position.ConverttoSF());
  }
  void draw(sf::RenderWindow &window)
  {
    window.draw(sprite);
  }

  friend bool operator==(const Boid &a, const Boid &b);
  friend bool operator!=(const Boid &a, const Boid &b);
};

bool operator==(const Boid &a, const Boid &b)
{
  return (a.position.x == b.position.x) && (a.position.y == b.position.y) && (a.velocity.x == b.velocity.x) && (a.velocity.y == b.velocity.y) && (a.acceleration.x == b.acceleration.x) && (a.acceleration.y == b.acceleration.y);
}

bool operator!=(const Boid &a, const Boid &b)
{
  return !(a == b);
}

int main()
{
  srand(time(0));
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "BOIDS");
  window.setFramerateLimit(30);

  tgui::Gui gui{window};
  auto alignmentSlider = tgui::Slider::create();
  alignmentSlider->setPosition(20, 560);
  alignmentSlider->setSize(200, 20);
  alignmentSlider->setMinimum(0);
  alignmentSlider->setMaximum(100);
  alignmentSlider->setValue(50);
  gui.add(alignmentSlider);

  auto cohesionSlider = tgui::Slider::create();
  cohesionSlider->setPosition(540, 560);
  cohesionSlider->setSize(200, 20);
  cohesionSlider->setMinimum(0);
  cohesionSlider->setMaximum(100);
  cohesionSlider->setValue(50);
  gui.add(cohesionSlider);

  auto separationSlider = tgui::Slider::create();
  separationSlider->setPosition(280, 560);
  separationSlider->setSize(200, 20);
  separationSlider->setMinimum(0);
  separationSlider->setMaximum(100);
  separationSlider->setValue(60);
  gui.add(separationSlider);

  auto alignmentText = tgui::Label::create();
  alignmentText->setSize(70, 40);
  alignmentText->setPosition(alignmentSlider->getPosition().x + alignmentSlider->getSize().x + 10, alignmentSlider->getPosition().y);
  alignmentText->setText(std::to_string((int)alignmentSlider->getValue()));
  alignmentText->setTextSize(20);
  auto a = alignmentText->getSharedRenderer();
  a->setTextColor(tgui::Color(255, 255, 255));
  gui.add(alignmentText);

  auto cohesionText = tgui::Label::create();
  cohesionText->setSize(70, 40);
  cohesionText->setPosition(cohesionSlider->getPosition().x + cohesionSlider->getSize().x + 10, cohesionSlider->getPosition().y);
  cohesionText->setText(std::to_string((int)cohesionSlider->getValue()));
  cohesionText->setTextSize(20);

  gui.add(cohesionText);

  auto separationText = tgui::Label::create();
  separationText->setSize(70, 40);
  separationText->setPosition(separationSlider->getPosition().x + separationSlider->getSize().x + 10, separationSlider->getPosition().y);
  separationText->setText(std::to_string((int)separationSlider->getValue()));
  separationText->setTextSize(20);

  gui.add(separationText);

  auto resetButton = tgui::Button::create();
  resetButton->setPosition(window.getSize().x - 80, window.getSize().y - 45);
  resetButton->setSize(55, 25);
  resetButton->setText("Reset");
  auto resetButtonRenderer = resetButton->getSharedRenderer();
  resetButtonRenderer->setTextColor(tgui::Color(255, 255, 255));
  resetButtonRenderer->setBackgroundColor(tgui::Color(0, 0, 0, 0));
  gui.add(resetButton);
  Boid boids[200];

  bool alignmentSliderMoved = false;
  bool cohesionSliderMoved = false;
  bool separationSliderMoved = false;
  bool buttonPressed = false;
  bool mouseDraw = false;

  std::vector<sf::CircleShape> vec;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {

      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::MouseButtonPressed:
      {
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
        {
          if (resetButton->mouseOnWidget((sf::Vector2f)sf::Mouse::getPosition(window)))
          {
            buttonPressed = true;
          }
          if (sf::Mouse::getPosition(window).y < window.getSize().y - 50)
          {
            mouseDraw = true;
          }
          if (alignmentSlider->mouseOnWidget(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
          {
            alignmentSliderMoved = true;
          }
          if (cohesionSlider->mouseOnWidget(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
          {
            cohesionSliderMoved = true;
          }
          if (separationSlider->mouseOnWidget(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
          {
            separationSliderMoved = true;
          }
        }
        break;
        }
        break;
      }
      case sf::Event::MouseButtonReleased:
      {
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
        {
          mouseDraw = false;
          alignmentSliderMoved = false;
          cohesionSliderMoved = false;
          separationSliderMoved = false;
          buttonPressed = false;
        }
        break;
        }
        break;
      }
      }
      if (mouseDraw && event.type == sf::Event::MouseMoved)
      {
        sf::CircleShape shape(3);
        shape.setPosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
        vec.push_back(shape);
      }
    }
    if (alignmentSliderMoved)
    {
      alignmentSlider->setValue(sf::Mouse::getPosition(window).x / 2 - alignmentSlider->getPosition().x / 2);
    }
    if (cohesionSliderMoved)
    {
      cohesionSlider->setValue(sf::Mouse::getPosition(window).x / 2 - cohesionSlider->getPosition().x / 2);
    }
    if (separationSliderMoved)
    {
      separationSlider->setValue(sf::Mouse::getPosition(window).x / 2 - separationSlider->getPosition().x / 2);
    }

    if (resetButton->mouseOnWidget((sf::Vector2f)sf::Mouse::getPosition(window)))
    {
      resetButtonRenderer->setTextColor(tgui::Color(255, 0, 255));
    }
    else
      resetButtonRenderer->setTextColor(tgui::Color(255, 255, 255));

    if (buttonPressed)
    {
      vec.clear();
      alignmentSlider->setValue(0);
      cohesionSlider->setValue(0);
      separationSlider->setValue(0);
      alignmentSlider->setValue(50);
      separationSlider->setValue(60);
      cohesionSlider->setValue(50);
    }

    alignmentText->setText(std::to_string((int)alignmentSlider->getValue()));
    cohesionText->setText(std::to_string((int)cohesionSlider->getValue()));
    separationText->setText(std::to_string((int)separationSlider->getValue()));

    window.clear();
    for (auto i : vec)
    {
      window.draw(i);
    }
    for (auto &i : boids)
    {
      i.flock(boids, vec, alignmentSlider->getValue(), cohesionSlider->getValue(), separationSlider->getValue());
      i.AvoidEdges();
      i.update();
      i.draw(window);
    }
    gui.draw();
    window.display();
  }
  return 0;
}
