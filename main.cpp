#include <iostream>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>
#include <set>
#include <vector>
#include <SFML/Window.hpp>
#include "agony.hpp"
#include "designation_menu.hpp"
struct point;
using namespace std;
int main() {
  sf::RenderWindow r(sf::VideoMode(520, 256), "Symmetricity");
  bool menumode = false;
  Agony e;
  sf::View q;
  q.setSize(520, 256);
  q.setCenter(0, 0);
  r.setView(q);
  Menu menustuff;
  e.setActivityCallback([&menustuff](int a) {menustuff.set_selected_item(a); });
  menustuff.addItem("Dig", [&e]() {
    e.set_designation_type(0);
  });
  menustuff.addItem("Up/Down staircase", [&e]() {e.set_designation_type(1); });
  menustuff.addItem("Down staircase", [&e]() {e.set_designation_type(2); });
  menustuff.addItem("Up staircase", [&e]() {e.set_designation_type(3); });
  menustuff.addItem("Remove Designation", [&e]() {e.set_designation_type(4); });

  /*e.set_thing(1, 1, 0);
  e.set_thing(1, 2, 0);
  e.set_thing(1, 1, -1);
  e.set_thing(5,5,0);*/
  while (r.isOpen()) {
    sf::Event event;
    while (r.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        r.close();
      if (event.type == sf::Event::Resized) {
        q.setSize(r.getSize().x, r.getSize().y);
        r.setView(q);
      }
      if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i g = sf::Mouse::getPosition(r);
        auto q = r.mapPixelToCoords(g);
        q.x /= 10;
        q.y /= 10;
        e.mouse_over(q); //tells the thing to display a circle where the mouse is over.
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == 1) {

          menumode = !menumode;
          continue;
        }
        if (menumode) {
          menustuff.onclick(r.mapPixelToCoords(sf::Mouse::getPosition(r)));
        } else {
          sf::Vector2i g = sf::Mouse::getPosition(r);
          auto q = r.mapPixelToCoords(g);
          q.x /= 10;
          q.y /= 10;
          e.long_desig(q);
        }
      }
      if (event.type == sf::Event::KeyPressed) {
        int times = 1;
        if (event.key.shift)
          times = 10;
        switch (event.key.code) {
        case sf::Keyboard::Right:
          if (event.key.control) {
            e.move_over(1, 0);
          } else
            for (int i = 0; i < times; i++)
              e.increment_cursor_x();
          break;
        case sf::Keyboard::Left:
          if (event.key.control)
            e.move_over(-1, 0);
          else
            for (int i = 0; i < times; i++)
              e.decrement_cursor_x();
          break;
        case sf::Keyboard::Up:
          if (event.key.control)
            e.move_over(0, -1);
          else
            for (int i = 0; i < times; i++)
              e.decrement_cursor_y();
          break;
        case sf::Keyboard::Down:
          if (event.key.control)
            e.move_over(0, 1);
          else
            for (int i = 0; i < times; i++)
              e.increment_cursor_y();
          break;
        case sf::Keyboard::Period:
          e.increase_z();
          break;
        case sf::Keyboard::Comma:
          e.decrease_z();
          break;
        case sf::Keyboard::X:
          e.add_x_symmetry_at_cursor();
          break;
        case sf::Keyboard::Y:
          e.add_y_symmetry_at_cursor();
          break;
        case sf::Keyboard::Subtract:
        case sf::Keyboard::Dash:
          e.decrease_activity();
          break;
        case sf::Keyboard::Add:
        case sf::Keyboard::Equal:
          e.increase_activity();
          break;
        case sf::Keyboard::Space:
          e.designate();
          break;
        case sf::Keyboard::R:
          e.add_radial_symmetry_at_cursor();
          break;
        case sf::Keyboard::Return:
          if (event.key.shift)
            e.set_circle();
          e.long_desig();
          break;
        case sf::Keyboard::F5: //TODO Write a method to select a file to write to.
          e.write_file_output("output.csv");
        }
      }
    }
    r.clear();
    if (!menumode) {
      r.draw(e);
    } else {
      r.draw(menustuff);
    }
    r.display();
  }
  return 0;
}
