#pragma once

#include <set>
#include <vector>
#include <iostream>
#include <cstdint>
#include "rectangle.hpp" 

using Id = uintptr_t; 

class Intersection
{
    Rectangle intersection_shape;
    std::set<Id> intersecting_rectangles;

public:
    // Constructor
    Intersection(const Rectangle &shape, const std::set<Id> &ids);

    // Function to compute intersections
    static std::set<Intersection> get_intersections(const std::vector<Rectangle> &inputs);

    // Friend declarations for operator overloads
    friend bool operator==(const Intersection &lhs, const Intersection &rhs);
    friend std::ostream &operator<<(std::ostream &os, const Intersection &inter);
    friend bool operator<(const Intersection &lhs, const Intersection &rhs);
    friend std::ostream &operator<<(std::ostream &os, const std::set<Intersection> &s);
};

// Operator overloads that might interact with other objects
bool operator==(const Intersection &lhs, const Intersection &rhs);
bool operator<(const Intersection &lhs, const Intersection &rhs);
std::ostream &operator<<(std::ostream &os, const Intersection &inter);
std::ostream &operator<<(std::ostream &os, const std::set<Intersection> &s);
std::ostream &operator<<(std::ostream &os, const std::set<Id> &s);