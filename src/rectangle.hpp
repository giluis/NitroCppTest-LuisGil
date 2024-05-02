#pragma once 

#include <boost/json.hpp>
#include <iostream>
#include <optional>
#include <cassert>


// u_int32_t gives us a concrete size that is machine-independent 
struct RectCoors {
    uint32_t x;
    uint32_t y;
    uint32_t w;
    uint32_t h;
};

class Rectangle {
public:
    uint32_t m_x;
    uint32_t m_y;
    uint32_t m_w;
    uint32_t m_h;

	// Why not accepting 4 uint32_t args? Because I'm very prone to mix up the order of the arguments
    // Using RectCoors allows using designated initializer lists, which prevents order mess-ups
    Rectangle(RectCoors const &coors);
    static std::optional<Rectangle> create(boost::json::value const & v);

    std::optional<Rectangle> intersect(const Rectangle &other) const;
};

// Operator overloads
bool operator==(const Rectangle &lhs, const Rectangle &rhs);
std::ostream &operator<<(std::ostream &os, const Rectangle &rect);
std::ostream &operator<<(std::ostream &os, const std::vector<Rectangle> &v);