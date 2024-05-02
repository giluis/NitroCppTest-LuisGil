#include <functional>
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdint>
#include <deque>
// #include "rectangle.hpp"
#include "intersection.hpp"

using std::vector, std::string;

using Id = uintptr_t;

// Constructor that accepts an r-value reference to a vector
Intersection::Intersection(const Rectangle &shape, const std::set<Id> &ids)
    : intersection_shape(shape), intersecting_rectangles(ids) {}

/**
 * This function will calculate all the intersections between the Rectangles given as input,
 * as well as intersections between those intersections and the remaining rectangles.
 * 
 * Algorithm:
 * 1. Compute 1st degree intersections 
 * 2. For each such intersection, push to a queue intersections with other rectangles that are not already involved in said intersection
 * 3. Keep popping from the q until there are no intersections left
 * 
*/
std::set<Intersection> Intersection::get_intersections(vector<Rectangle> const &inputs)
{

	// Compute 1st degree intersections
    // A solution that only requires a single loop is possible
    // but would require considering single-rectangle intersections. This approach feels more understandable_
    // perhaps an intersection of a single rectangle
    std::deque<Intersection> q;
    for (Id i = 1; i <= inputs.size(); i += 1)
    {
        for (Id j = i + 1; j <= inputs.size(); j += 1)
        {
            // Ids are 1 based
            auto inter = inputs[i - 1].intersect(inputs[j - 1]);
            if (inter.has_value())
            {
                q.push_back(Intersection(*inter, {i, j}));
            }
        }
    }

	// Consider a solution using only 1 collection rather than 2 (q and all_intersections)
    // It's more performant, but would be more complex
    std::set<Intersection> all_intersections(q.begin(), q.end());
    while (!q.empty())
    {
        // pop from queue
        Intersection inter = q.front();
        q.pop_front();

        // for each of the rectangles in input
        for (Id id = 1; id <=inputs.size(); id += 1)
        {

			// Ids are 1-based
            auto rect = inputs[id - 1];
            // if the intersection doesn't already include the current rectangle
            if (inter.intersecting_rectangles.find(id) == inter.intersecting_rectangles.end())
            {
                auto new_inter_shape = inter.intersection_shape.intersect(rect);
                // if there is an intersection between the intersection and the current rectangle
                if (new_inter_shape.has_value())
                {
                    std::set<Id> new_ids(inter.intersecting_rectangles);
                    new_ids.insert(id);
                    // if no other intersections between the same rectangles have been found so far
                    // covers the test case "two_single_overlaps_and_one_triple"
                    if (!std::any_of(all_intersections.cbegin(), all_intersections.cend(), [&new_ids](const Intersection &i)
                                     { return i.intersecting_rectangles == new_ids; }))
                    {
                        auto new_inter = Intersection(*new_inter_shape, new_ids);
                        q.push_back(new_inter);
                        all_intersections.insert(new_inter);
                    }
                }
            }
        }
    }
    return all_intersections;
}

// comparing intersecting ids would be sufficient, since no 2 intersections can have the same 2 ids
// however, this extra check might prevent some bugs
bool operator==(const Intersection &lhs, const Intersection &rhs)
{
    return lhs.intersecting_rectangles == rhs.intersecting_rectangles && lhs.intersection_shape == rhs.intersection_shape;
}

// ordering is necessary for consistent test case results
bool operator<(const Intersection &lhs, const Intersection &rhs)
{
    return lhs.intersecting_rectangles < rhs.intersecting_rectangles;
}

std::ostream &operator<<(std::ostream &os, const Intersection &inter)
{
    os << "Intersection Shape: " << inter.intersection_shape << ", IDs: ";
    for (const auto &id : inter.intersecting_rectangles)
    {
        os << id << " ";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::set<Intersection> &s)
{
    for (auto const &inter : s)
    {
        os << "\t" << "Between rectangle " << inter.intersecting_rectangles << " at " << inter.intersection_shape << std::endl;
    }
    os << "]";
    return os;
}

std::ostream &operator<<(std::ostream &os, const std::set<Id> &s)
{
    if (s.size() < 2)
    {
        os << "{}";
    }
    else
    {
        for (auto it = s.begin(); it != s.end(); ++it)
        {
            if (it == s.begin()) {
                os << *it;
            } else if (std::next(it) == s.end())
            {
                os << " and " << *it;
            } else
            {
                os << ", " << *it;
            }
        }
    }
    return os;
}

/* ============================================================== */
/*                             TESTS                              */
/* ============================================================== */
