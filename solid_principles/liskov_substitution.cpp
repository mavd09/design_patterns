/*
 * Liskov Substitution Principle
 * 
 * Objects of a superclass shall be replaceable
 * with objects of its subclasses without breaking
 * the applicationi.
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

class Rectangle {
protected:
  int width, height;

public:
  Rectangle(int width, int height): width(width), height(height) {}

  int getWidth() const {
    return width;
  }

  virtual void setWidth(int width) {
    this->width = width;
  }

  int getHeight() const {
    return height;
  }

  virtual void setHeight(int height) {
    this->height = height;
  }

  int getArea() const {
    return width * height;
  }
};

class Square : public Rectangle {
public:
  Square(int size): Rectangle(size, size) {}

  void setWidth(int width) override {
    this->width = this->height = width;
  }

  void setHeight(int height) override {
    this->height = this->width = height;
  }
};

void process(Rectangle& r) {
  int w = r.getWidth();
  r.setHeight(10);

  cout << "expected area = " << (w * 10)
       << ", got " << r.getArea() << endl;;
}

int main() {
  Rectangle r{3, 4};
  process(r);

  Square sq{5};
  process(sq);

  return 0;
}

