/*
 * Open-Closed Principle
 * 
 * Software entities should be open for extension
 * but closed for modification.
 */

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

enum Color { red, green, blue };
enum Size { small, medium, large };

struct Product {
  string name;
  Color color;
  Size size;
};

// We must avoid repeated code that every time we should
// add new feature/functionality requires to copy and paste
// previous code and it makes the code unmaintainable
struct ProductFilter {
  vector<Product*> by_color(vector<Product*> items, Color color) {
    vector<Product*> result;
    for(auto& item : items) {
      if(item->color == color) {
        result.push_back(item);
      }
    }
    return result;
  }

  vector<Product*> by_size(vector<Product*> items, Size size) {
    vector<Product*> result;
    for(auto& item : items) {
      if(item->size == size) {
        result.push_back(item);
      }
    }
    return result;
  }

  vector<Product*> by_color_and_size(vector<Product*> items, 
                                     Color color, Size size) {
    vector<Product*> result;
    for(auto& item : items) {
      if(item->color == color && item->size == size) {
        result.push_back(item);
      }
    }
    return result;
  }
};

template<typename T> struct Specification {
  virtual bool is_satisfied(T* item) = 0;
};

template<typename T> struct AndSpecification : Specification<T> {
  Specification<T>& first;
  Specification<T>& second;

  AndSpecification(Specification<T>& first, Specification<T>& second) : 
    first(first), second(second) { }

  bool is_satisfied(T* item) override {
    return first.is_satisfied(item) && second.is_satisfied(item);
  }
};

template<typename T> struct Filter {
  virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};

struct ColorSpecification : Specification<Product> {
  Color color;

  ColorSpecification(Color color) : color(color) {}

  bool is_satisfied(Product* item) override {
    return item->color == color;
  }
};

struct SizeSpecification : Specification<Product> {
  Size size;

  SizeSpecification(Size size) : size(size) { }

  bool is_satisfied(Product* item) override {
    return item->size == size;
  }
};

struct BetterProductFilter : Filter<Product> {
  vector<Product*> filter(vector<Product*> items, 
                          Specification<Product>& spec) override {
    vector<Product*> result;
    for(auto& item : items) {
      if(spec.is_satisfied(item)) {
        result.push_back(item);
      }
    }
    return result;
  }
};

int main() {
  Product apple{"Apple", Color::green, Size::small};
  Product tree{"Tree", Color::green, Size::large};
  Product house{"House", Color::blue, Size::large};
  
  vector<Product*> items{&apple, &tree, &house};
  
  ProductFilter pf;
  
  auto green_items = pf.by_color(items, Color::green);
  for(auto& item : green_items) {
    cout << item->name << " is green" << endl;
  }
  
  auto green_and_large_items = 
    pf.by_color_and_size(items, Color::green, Size::large);
  for(auto& item : green_and_large_items) {
    cout << item->name << " is green and large" << endl;
  }

  cout << "=====" << endl;

  BetterProductFilter bpf;  
  
  ColorSpecification green(Color::green);
  for(auto& item : bpf.filter(items, green)) {
    cout << item->name << " is green" << endl; 
  }

  SizeSpecification large(Size::large);
  AndSpecification<Product> green_and_large(green, large);
  for(auto& item : bpf.filter(items, green_and_large)) {
    cout << item->name << " is green and large" << endl;
  }

  return 0;
}

