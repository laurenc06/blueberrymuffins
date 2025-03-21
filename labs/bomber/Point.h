#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <cmath>

struct Point {
  int lat;  // Latitude
  int lng;  // Longitude

  Point() {
    // Beware of junk values!
  }

  Point(int lat, int lng) {
    this->lat = lat;
    this->lng = lng;
  }

  Point& operator=(const Point& other) { // explicitly defined a copy assignment operator
    if (this != &other) {
      this->lat = other.lat;
      this->lng = other.lng;
    }
    return *this;
  }

  Point(const Point& other) { //copy constructor
    this->lat = other.lat;
    this->lng = other.lng;
  }
};

// Operators for easier (and consistent) input and output:
std::ostream& operator << (std::ostream& stream, const Point& point);
std::istream& operator >> (std::istream& stream, Point& point);

#endif
