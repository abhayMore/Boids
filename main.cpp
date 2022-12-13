#include "boids.hpp"

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>

#define WIDTH 900
#define HEIGHT 600

int main() {
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
  alignmentText->setPosition(
      alignmentSlider->getPosition().x + alignmentSlider->getSize().x + 10,
      alignmentSlider->getPosition().y);
  alignmentText->setText(std::to_string((int)alignmentSlider->getValue()));
  alignmentText->setTextSize(20);

  auto textColor = alignmentText->getSharedRenderer();
  textColor->setTextColor(tgui::Color(255, 255, 255));
  gui.add(alignmentText);

  auto cohesionText = tgui::Label::create();
  cohesionText->setSize(70, 40);
  cohesionText->setPosition(
      cohesionSlider->getPosition().x + cohesionSlider->getSize().x + 10,
      cohesionSlider->getPosition().y);
  cohesionText->setText(std::to_string((int)cohesionSlider->getValue()));
  cohesionText->setTextSize(20);
  gui.add(cohesionText);

  auto separationText = tgui::Label::create();
  separationText->setSize(70, 40);
  separationText->setPosition(
      separationSlider->getPosition().x + separationSlider->getSize().x + 10,
      separationSlider->getPosition().y);
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

  bool alignmentSliderMoved = false;
  bool cohesionSliderMoved = false;
  bool separationSliderMoved = false;
  bool buttonPressed = false;
  bool mouseDraw = false;

  Boids boids[200];

  std::vector<sf::CircleShape> vec;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::MouseButtonPressed: {
          switch (event.mouseButton.button) {
            case sf::Mouse::Left: {
              if (resetButton->isMouseOnWidget(
                      (sf::Vector2f)sf::Mouse::getPosition(window))) {
                buttonPressed = true;
              }
              if (sf::Mouse::getPosition(window).y < window.getSize().y - 50) {
                mouseDraw = true;
              }
              if (alignmentSlider->isMouseOnWidget(
                      sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                alignmentSliderMoved = true;
              }
              if (cohesionSlider->isMouseOnWidget(
                      sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                cohesionSliderMoved = true;
              }
              if (separationSlider->isMouseOnWidget(
                      sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                separationSliderMoved = true;
              }
            } break;
          }
          break;
        }
        case sf::Event::MouseButtonReleased: {
          switch (event.mouseButton.button) {
            case sf::Mouse::Left: {
              mouseDraw = false;
              alignmentSliderMoved = false;
              cohesionSliderMoved = false;
              separationSliderMoved = false;
              buttonPressed = false;
            } break;
          }
          break;
        }
      }
      if (mouseDraw && event.type == sf::Event::MouseMoved) {
        sf::CircleShape shape(3);
        shape.setPosition(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
        vec.push_back(shape);
      }
    }
    if (alignmentSliderMoved) {
      alignmentSlider->setValue(sf::Mouse::getPosition(window).x / 2 -
                                alignmentSlider->getPosition().x / 2);
    }
    if (cohesionSliderMoved) {
      cohesionSlider->setValue(sf::Mouse::getPosition(window).x / 2 -
                               cohesionSlider->getPosition().x / 2);
    }
    if (separationSliderMoved) {
      separationSlider->setValue(sf::Mouse::getPosition(window).x / 2 -
                                 separationSlider->getPosition().x / 2);
    }

    if (resetButton->isMouseOnWidget(
            (sf::Vector2f)sf::Mouse::getPosition(window))) {
      resetButtonRenderer->setTextColor(tgui::Color(255, 0, 255));
    } else
      resetButtonRenderer->setTextColor(tgui::Color(255, 255, 255));

    if (buttonPressed) {
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
    for (auto i : vec) {
      window.draw(i);
    }
    for (auto &i : boids) {
      i.flock(boids, vec, alignmentSlider->getValue(),
              cohesionSlider->getValue(), separationSlider->getValue());
      i.AvoidEdges(HEIGHT, WIDTH);
      i.update();
      i.draw(window);
    }
    gui.draw();
    window.display();
  }
  return 0;
}
