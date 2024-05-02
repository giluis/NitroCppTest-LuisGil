#include <boost/json.hpp>
#include <vector>
#include <cassert>
#include <optional>
#include <iostream>
#include "rectangle.hpp"

using std::vector, std::string;

Rectangle::Rectangle(RectCoors const &coors)
{


    // Negative starting coordinates seem to not be used in the examples
    // I will assume they will always be positive
    // uint32_t fits these criteria by definition
    // assert(coors.x >= 0 && coors.y >= 0)
    
    // zero height or width rectangles are not rectangles, but lines
    assert(coors.w > 0 && coors.h > 0);

    m_x = coors.x;
    m_y = coors.y;
    m_w = coors.w;
    m_h = coors.h;
}

// Creates a Rectangle from a json object, according to business rules
// Object should have only 4 fields, x,y,w and h, each of those with a uint32_t-parsable value
std::optional<Rectangle> Rectangle::create(boost::json::value const & v) {
    using namespace boost::json;

    if (!v.is_object()) {
        return std::nullopt;
    }

    const object& obj = v.as_object();
    if (obj.size() != 4 || 
        !obj.contains("x") || !obj.contains("y") || 
        !obj.contains("w") || !obj.contains("h")) {
        return std::nullopt;
    }

    try {
        uint32_t x = value_to<uint32_t>(obj.at("x"));
        uint32_t y = value_to<uint32_t>(obj.at("y"));
        uint32_t w = value_to<uint32_t>(obj.at("w"));
        uint32_t h = value_to<uint32_t>(obj.at("h"));

        return Rectangle({.x = x, .y = y, .w = w, .h = h}); 

    } catch (const std::exception&) {
        // Catch any type mismatch or other JSON errors
        return std::nullopt;
    }
}

// Intersection is computed by projecting the Rectangle onto the x and y-axis
// and then calculating range intersection on those axis
std::optional<Rectangle> Rectangle::intersect(const Rectangle &other) const
{
    // Determine the start of the intersection range
    uint32_t start_x = std::max(m_x, other.m_x);
    // Determine the end of the intersection range
    uint32_t end_x = std::min(m_x + m_w, other.m_x + other.m_w);

	// No intersection found in x-axis
    if (start_x >= end_x)
    {
        return std::nullopt;
    }

    // Determine the start of the intersection range
    uint32_t start_y = std::max(m_y, other.m_y);
    // Determine the end of the intersection range
    uint32_t end_y = std::min(m_y + m_h, other.m_y + other.m_h);

	// No intersection found in y-axis
    if (start_y >= end_y)
    {
        return std::nullopt;
    }

    return Rectangle({
        .x = start_x,
        .y = start_y,
        .w = end_x - start_x,
        .h = end_y - start_y,
    });
}

bool operator==(const Rectangle &lhs, const Rectangle &rhs)
{
    return lhs.m_x == rhs.m_x && lhs.m_y == rhs.m_y && lhs.m_w == rhs.m_w && lhs.m_h == rhs.m_h;
}

std::ostream &operator<<(std::ostream &os, const Rectangle &rect)
{
    os << "Rectangle(x=" << rect.m_x << ", y=" << rect.m_y << ", w=" << rect.m_w << ", h=" << rect.m_h << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::vector<Rectangle> &v)
{
    for(uintptr_t i = 0; i < v.size(); i += 1 ) {
        auto rect = v[i];
        os << "\t" << i + 1 << ": Rectangle at (" << rect.m_x << "," <<  rect.m_y << "), w=" << rect.m_w << ", h=" << rect.m_h << ".\n";
    }
    return os;
}
