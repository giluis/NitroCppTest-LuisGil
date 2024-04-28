#include <boost/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>

class RectCoors
{
public:
    int x;
    int y;
    int w;
    int h;
};

using std::vector, std::string;

class Rectangle
{

public:
    int x;
    int y;
    int w;
    int h;

    friend bool operator==(const Rectangle& lhs, const Rectangle& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Intersection& inter);

    Rectangle(RectCoors const &coors)
    {

        x = coors.x;
        y = coors.y;
        w = coors.w;
        h = coors.h;
        // negative starting coordinates seem to not be used in the examples
        // I will assume they will always be positive
        assert(x >= 0);
        assert(y >= 0);
        // zero width rectangles are not rectangles, but lines
        assert(w > 0);
        // zero height rectangles are not rectangles, but lines
        assert(h > 0);
    }

    std::optional<Rectangle> intersect(Rectangle &other)
    {
        return std::nullopt;
    }

};


using Id = uintptr_t;

class Intersection
{

    Rectangle intersection_shape;
    std::set<Id> intersecting_rectangles;

public:
    friend bool operator==(const Intersection& lhs, const Intersection& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Intersection& inter);

    // Constructor that accepts an r-value reference to a vector
    Intersection(const Rectangle &shape, const std::set<Id> &ids)
        : intersection_shape(shape), intersecting_rectangles(ids) {}
};

struct TestCase
{
    vector<Rectangle> inputs;
    vector<Intersection> expected_result;
};

boost::json::value read_json_from_file(std::string file_path)
{
    // Read JSON from file
    std::ifstream ifs(file_path);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return 1;
    }

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close(); // Close the file stream

    // Parse the JSON string
    boost::json::value jv = boost::json::parse(str);
    return jv;
}

vector<Intersection> get_intersections(vector<Rectangle> const &inputs)
{

    return vector<Intersection>();
}

bool operator==(const Rectangle& lhs, const Rectangle& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h;
}

bool operator==(const Intersection& lhs, const Intersection& rhs) {
    return lhs.intersection_shape == rhs.intersection_shape && lhs.intersecting_rectangles == rhs.intersecting_rectangles;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1)
            os << ", ";
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::set<T>& vec) {
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1)
            os << ", ";
    }
    os << "]";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Intersection& inter) {
    os << "Intersection Shape: " << inter.intersection_shape << ", IDs: ";
    for (const auto& id : inter.intersecting_rectangles) {
        os << id << " ";
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
    os << "Rectangle(x=" << rect.x << ", y=" << rect.y << ", w=" << rect.w << ", h=" << rect.h << ")";
    return os;
}


/* ============================================================== */
/*                             TESTS                              */
/* ============================================================== */

class RectangleIntersectionTest
{

    /*

  0                   1
  0 1 2 3 4 5 6 7 8 9 0 1 2 3

                    [A]
00    o . . . . . o
 1    . Y Y Y Y Y Y [B]
 2    . Y X X X X X [C]   X1: intersection of A and C (shape of C)
 3    . Y X X X X X       X2: intersection of B and C (shape of C)
 4    . Y X X X X X       X3: intersection of A, B and C (shape of C)
 5    o Y X X X X X       Y1: intersection of A and B
 6
    */
    static TestCase adjacent_contained()
    {

        auto A = Rectangle({.x = 2, .y = 0, .w = 6, .h = 5});

        auto B = Rectangle({
            .x = A.x + 1,
            .y = A.y + 1,
            .w = A.w - 1,
            .h = A.h - 1,
        });

        auto C = Rectangle({
            .x = B.x + 1,
            .y = B.y + 1,
            .w = B.w - 1,
            .h = B.h - 1,
        });

        return TestCase{
            // A=0, B=1, C=2
            .inputs = {A, B, C},
            .expected_result = {
                // X1: intersetion of A and C (has shape of C)
                Intersection(C, {0, 2}),
                // X2: intersetion of B and C (has shape of C)
                Intersection(C, {1, 2}),
                // X3: intersection of A, B and C (has shape of C)
                Intersection(C, {0, 1, 2}),
                // X1: intersetion of A and B (has shape of B)
                Intersection(B, {0, 1}),
            }

        };
    }
    /*

  0                   1
  0 1 2 3 4 5 6 7 8 9 0 1 2 3

                    [A]
00    o . . . . . o
 1    .           .     [B]
 2    .   X X X X X . o
 3    .   X X X X X   .        [C]
 4    .   X X X T T Y Y . . . o
 5    o . X X X T T Y Y       .
 6        o . . Y Y Y Y       .
 7        	    .             .
 8              o . . . . . . o

    */
    static TestCase two_single_overlaps_and_one_triple()
    {
        Rectangle A({.x = 2,
                     .y = 0,
                     .w = 6,
                     .h = 5});

        Rectangle B({
            .x = A.x + A.w / 2 - 1,
            .y = A.y + A.h / 2,
            .w = A.w + 2,
            .h = A.h,
        });

        Rectangle C({
            .x = B.x + B.w / 2 - 1,
            .y = B.y + B.h / 2,
            .w = A.w + 1,
            .h = 5,
        });

        return TestCase{
            // A=0,B=1,C=2
            .inputs = {A, B, C},
            .expected_result = {
                // intersection X ,between A and B
                Intersection({{.x = B.x, .y = B.y, .w = A.x + A.w - B.x, .h = A.y + A.h - B.y}}, {0, 1}),
                // intersection Y ,between B and C
                Intersection({{.x = C.x, .y = C.y, .w = B.x + B.w - C.x, .h = B.y + B.h - C.y}}, {1, 2}),
                // intersection T1, between A, B and C
                Intersection({{.x = C.x, .y = C.y, .w = A.x + A.w - C.x, .h = A.y + A.h - C.y}}, {0, 1, 2}),
                // intersection T2, between A, and C (same shape as T1)
                Intersection({{.x = C.x, .y = C.y, .w = A.x + A.w - C.x, .h = A.y + A.h - C.y}}, {0, 1, 2}),
            }

        };
    }

    /*

    0                   1
    0 1 2 3 4 5 6 7 8 9 0 1 2 3

  A o . . . o
    .       .         B
    .   X X X . . . o
    .   X X X       .
    .   X X X       .         C
    o . X X X   Y Y Y . . . o
        o . . . Y Y Y       .
                o . . . . . o

    */
    static TestCase two_non_overlapping_intersections()
    {
        Rectangle A({
            .x = 1,
            .y = 1,
            .w = 4,
            .h = 5,
        });

        Rectangle B({
            .x = A.x + A.w / 2,
            .y = A.y + A.h / 2,
            .w = A.w + 2,
            .h = A.h,
        });

        Rectangle C({
            .x = B.x + B.w / 2 + 1,
            .y = B.y + B.h / 2 + 1,
            .w = B.w + 5,
            .h = 3,
        });

        return TestCase{
            // A = 0, B = 1, C = 2
            .inputs = {A, B, C},
            .expected_result = {
                // intersection X
                Intersection({{.x = B.x, .y = B.y, .w = A.x + A.w - B.x, .h = A.y + A.h - B.y}}, {0, 1}),
                // intersection Y
                Intersection({{.x = C.x, .y = C.y, .w = B.x + B.w - C.x, .h = B.y + B.h - C.y}}, {1, 2}),
            }

        };
    }

    /*
              [A]
    o . . . o
    .       .       [B]
    .   X X X . . o
    .   X X X     .
    o . X X X     .
        o . . . . o

    */
    static TestCase intersection2()
    {
        auto A = Rectangle({
            .x = 100,
            .y = 120,
            .w = 50,
            .h = 50,
        });

        auto B = Rectangle({
            .x = A.x + A.w / 2,
            .y = A.y + A.h / 2,
            .w = A.w,
            .h = A.h,
        });

        return TestCase{
            .inputs = {A, B},
            .expected_result = {Intersection({{.x = B.x, .y = B.y, .w = A.x + A.w - B.x, .h = A.y + A.h - B.y}}, {0, 1})}};
    }

public:
    static void runAll()
    {
        run(RectangleIntersectionTest::intersection2(), "Intersection of 2 rectangles");
    }

    static void run(const TestCase &test_case, string name)
    {
        auto actual = get_intersections(test_case.inputs);
        std::cout << "\n> " << name << "........";
        if (actual != test_case.expected_result)
        {
            std::cout << "Failed test\n"
                      << "\t expected: " << test_case.expected_result << "\n\t got: " << actual;
        }
        else
        {
            std::cout << "\nPassed!";
        }
    }
};


int main()
{
    // Path to the JSON file
    std::string file_path = "example.json";
    auto json = read_json_from_file(file_path);
    std::cout << "Read jsson: " << json;

    RectangleIntersectionTest::runAll();

    return 0;
}