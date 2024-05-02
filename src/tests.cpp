#include <iostream>
#include <vector>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <cassert>

#include "rectangle.hpp"
#include "intersection.hpp"

using std::vector, std::string;

template <typename I, typename O>
struct ITestCase
{
    I inputs;
    O expected;
};

// Total number of spaces between "> Test name" and "Passed" / "Failed"
// Necessary for proper readable alignment
const u_int8_t SPACING_DOTS = 60;

// returns excatly enough dots "....." so that all the "Passed" after the test names are aligned
string get_spacing_dots(const string &test_name)
{
    // arbitrary spacing difference
    assert(test_name.size() + 5 < SPACING_DOTS);
    std::ostringstream os;
    const string sep = " . ";
    uint8_t start = test_name.size();
    while (start % sep.size() != 0)
    {
        os << " ";
        start += 1;
    }
    for (int i = start; i < (SPACING_DOTS - SPACING_DOTS % sep.size()); i += sep.size())
    {
        os << sep;
    }
    string result = os.str();
    return result;
}

void print_test_case(bool passed, const string &name, std::function<string()> error_message)
{
    auto spacing_points = get_spacing_dots(name);
    std::cout << "\n    > " << name << spacing_points;
    if (passed)
    {
        std::cout << "Passed!\n";
        return;
    }
    std::cout << "FAILED \n"
              << error_message();
}

class RectangleTest
{

    using TestCase = ITestCase<std::pair<Rectangle, Rectangle>, std::optional<Rectangle>>;

    /*
    1
    1 2 3 4 5 6 7 8
                [A]
  00    o . . . o
   1    .       .       [B]
   2    .       . o . o
   3    .       . .   .
   4    .       . .   .
   5    o . . . . .   .
   6              o . o

    */
    static TestCase no_intersection_when_adjacent()
    {
        auto A = Rectangle({
            .x = 13,
            .y = 0,
            .w = 4,
            .h = 5,
        });

        auto B = Rectangle({
            .x = 18,
            .y = 2,
            .w = 2,
            .h = 4,
        });

        auto actual = A.intersect(B);
        auto expected = std::nullopt;

        return TestCase{
            .inputs = {A, B},
            .expected = std::nullopt};
    }

    /*
    1
    1 2 3 4 5 6 7 8
                [A]
  00    o . . . o
   1    .       .       [B]
   2    .       N . . o
   3    .       N     .
   4    .       N     .
   5    o . . . N     .
   6            o . . o

    */
    static TestCase no_intersection_when_overlapped_ends()
    {
        auto A = Rectangle({
            .x = 13,
            .y = 0,
            .w = 4,
            .h = 5,
        });

        auto B = Rectangle({
            .x = 17,
            .y = 2,
            .w = 3,
            .h = 4,
        });

        auto actual = A.intersect(B);
        return TestCase{
            .inputs = {A, B},
            .expected = std::nullopt};
    }

    /*
    1
    0 1 2 3 4 5 6 7 8
                [A]
  01    o . . . o
   2    .       .       [B]
   3    .   X X X . . o
   4    .   X X X     .
   5    .   X X X     .
   6    o . X X X     .
   7        o . . . . o

    */
    static TestCase intersection2()
    {
        auto A = Rectangle({
            .x = 12,
            .y = 1,
            .w = 4,
            .h = 5,
        });

        auto B = Rectangle({
            .x = 14,
            .y = 3,
            .w = 5,
            .h = 5,
        });

        auto actual = A.intersect(B);
        return TestCase{
            .inputs = {A, B},
            .expected = std::make_optional(Rectangle({.x = B.m_x, .y = B.m_y, .w = A.m_x + A.m_w - B.m_x, .h = A.m_y + A.m_h - B.m_y}))};
    }

    /*
    0
    0 1 2 3 4 5 6 7 8
                [A]
  00
   1    o . . . o
   2    .       .       [B]
   3    X X X X X o
   4    X X X X X .
   5    X X X X X .
   6    X X X X X o
   7

    */
    static TestCase partially_contained()
    {
        auto A = Rectangle({
            .x = 2,
            .y = 1,
            .w = 4,
            .h = 5,
        });

        auto B = Rectangle({
            .x = 2,
            .y = 3,
            .w = 5,
            .h = 3,
        });

        auto actual = A.intersect(B);
        return TestCase{
            .inputs = {A, B},
            .expected = std::make_optional(Rectangle({.x = B.m_x, .y = B.m_y, .w = A.m_w, .h = B.m_h}))};
    }

    /*
    0
    0 1 2 3 4 5 6 7 8
                [A]
  00
   1      o . . . o
   2      .       .       [B]
   3    o X X X X X o
   4    . X X X X X .
   5    . X X X X X .
   6    o X X X X X o
   7

    */
    static TestCase partially_contained_overflowing()
    {
        auto A = Rectangle({
            .x = 3,
            .y = 1,
            .w = 4,
            .h = 5,
        });

        auto B = Rectangle({
            .x = 2,
            .y = 3,
            .w = 6,
            .h = 3,
        });

        return TestCase{
            .inputs = {A, B},
            .expected = std::make_optional(Rectangle({.x = B.m_x + 1, .y = B.m_y, .w = B.m_w - 2, .h = B.m_h}))};
    }

    /*
    0
    0 1 2 3 4 5 6 7 8
                [A]
  00
   1    o . . . o
   2    .       .  [B]
   3    . X X X .
   4    . X X X .
   5    . X X X .
   6    o . . . o
   7

    */
    static TestCase fully_contained()
    {
        auto A = Rectangle({
            .x = 2,
            .y = 1,
            .w = 4,
            .h = 5,
        });

        auto B = Rectangle({
            .x = 3,
            .y = 3,
            .w = 2,
            .h = 2,
        });

        return TestCase{
            .inputs = {A, B},
            .expected = std::make_optional(B)};
    }

    /*
    0
    0 1 2 3 4 5 6 7 8
                [A]
  00
   1    X X X X X
   2    X X X X X  [B]
   3    X X X X X
   4    X X X X X
   5    X X X X X
   6    X X X X X
   7

    */
    static TestCase fully_overlapping()
    {
        auto A = Rectangle({
            .x = 2,
            .y = 1,
            .w = 4,
            .h = 5,
        });

        auto B = A;

        return TestCase{
            .inputs = {A, B},
            .expected = std::make_optional(B)};
    }

public:
    static void runAll()
    {
        std::cout << "--> Rectangle Tests";
        run_and_reverse(no_intersection_when_adjacent(), "No intersection when adjacent");
        run_and_reverse(intersection2(), "Simple intersection of 2 rects");
        run_and_reverse(no_intersection_when_overlapped_ends(), "No intersction happens when only ends overlap");
        run_and_reverse(no_intersection_when_adjacent(), "No intersection when adjacent and not overlapping");
        run_and_reverse(partially_contained(), "Partially contained");
        run_and_reverse(fully_contained(), "Fully contained");
        run_and_reverse(fully_overlapping(), "Fully overlapping");
        run_and_reverse(partially_contained_overflowing(), "Partially contained and overflowing");
        std::cout << "\n";
    }

    static void run_and_reverse(const TestCase &test, string name)
    {
        auto actual = test.inputs.first.intersect(test.inputs.second);

        print_test_case(check_passed(test.expected, actual), name, [&test, &actual]()
                        {
            std::ostringstream os;
            os << "\t expected: " << *test.expected << "\n\t got: " << *actual;
            return os.str(); });

        string name_rev = name + " rev";
        auto actual_rev = test.inputs.second.intersect(test.inputs.first);

        print_test_case(check_passed(test.expected, actual_rev), name_rev, [&test, &actual_rev]()
                        {
            std::ostringstream os;
            os << "\t expected: " << *test.expected << "\n\t got: " << *actual_rev;
            return os.str(); });
    }

    static bool check_passed(const std::optional<Rectangle> &expected, const std::optional<Rectangle> &actual)
    {

        return !actual.has_value() && !expected.has_value() || (actual.has_value() && expected.has_value() && *actual == *expected);
    }
};

class IntersectionTest
{

    using TestCase = ITestCase<vector<Rectangle>, std::set<Intersection>>;
    /*

      0                   1
      0 1 2 3 4 5 6 7 8 9 0 1 2 3

        [A]
    00    o . . . . . o
     1    .           .
     2    .           .
     3    .           .    No intersections
     4    .           .
     5    o . o . . o o . . . . o   [C]
     6        .     . .         .
     7        o . . o .         .
     8       [B]      o . . . . o
        */
    static TestCase line_and_corner_dont_intersect()
    {

        auto A = Rectangle({.x = 2, .y = 0, .w = 6, .h = 5});

        auto B = Rectangle({.x = 4,
                            .y = A.m_y + A.m_h,
                            .w = 3,
                            .h = 2});

        auto C = Rectangle({.x = A.m_x + A.m_w,
                            .y = A.m_y + A.m_h,
                            .w = 5,
                            .h = 3});

        return TestCase{
            // A=1, B=2, C=3
            .inputs = {A, B, C},
            .expected = {/*No intersections*/}

        };
    }
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

        auto B = Rectangle({.x = 3,
                            .y = 1,
                            .w = 5,
                            .h = 4});

        auto C = Rectangle({.x = 4,
                            .y = 2,
                            .w = 4,
                            .h = 3});

        return TestCase{
            // A=1, B=2, C=3
            .inputs = {A, B, C},
            .expected = {
                // X1: intersetion of A and C (has shape of C)
                Intersection(C, {1, 3}),
                // X2: intersetion of B and C (has shape of C)
                Intersection(C, {2, 3}),
                // X3: intersection of A, B and C (has shape of C)
                Intersection(C, {1, 2, 3}),
                // X1: intersetion of A and B (has shape of B)
                Intersection(B, {1, 2}),
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

        Rectangle B({.x = 4,
                     .y = 2,
                     .w = 6,
                     .h = 4});

        Rectangle C({.x = 7,
                     .y = 4,
                     .w = 7,
                     .h = 4});

        return TestCase{
            // A=1,B=2,C=3
            .inputs = {A, B, C},
            .expected = {
                // intersection X ,between A and B
                Intersection({{.x = B.m_x, .y = B.m_y, .w = 4, .h = 3}}, {1, 2}),
                // intersection T2, between A, and C (same shape as between A, B and C)
                Intersection({{.x = C.m_x, .y = C.m_y, .w = 1, .h = 1}}, {1, 3}),
                // intersection Y ,between B and C
                Intersection({{.x = C.m_x, .y = C.m_y, .w = 3, .h = 2}}, {2, 3}),
                // intersection T, between A, B and C (same shape as between A and C)
                Intersection({{.x = C.m_x, .y = C.m_y, .w = 1, .h = 1}}, {1, 2, 3})

            }};
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
        Rectangle A({.x = 1,
                     .y = 1,
                     .w = 4,
                     .h = 5});

        Rectangle B({.x = A.m_x + A.m_w / 2,
                     .y = A.m_y + A.m_h / 2,
                     .w = A.m_w + 2,
                     .h = A.m_h});

        Rectangle C({.x = B.m_x + B.m_w / 2 + 1,
                     .y = B.m_y + B.m_h / 2 + 1,
                     .w = B.m_w + 5,
                     .h = 3});

        return TestCase{
            // A = 1, B = 2, C = 3
            .inputs = {A, B, C},
            .expected = {
                // intersection X
                Intersection({{.x = B.m_x, .y = B.m_y, .w = A.m_x + A.m_w - B.m_x, .h = A.m_y + A.m_h - B.m_y}}, {1, 2}),
                // intersection Y
                Intersection({{.x = C.m_x, .y = C.m_y, .w = B.m_x + B.m_w - C.m_x, .h = B.m_y + B.m_h - C.m_y}}, {2, 3}),
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
        auto A = Rectangle({.x = 100,
                            .y = 120,
                            .w = 50,
                            .h = 50});

        auto B = Rectangle({.x = A.m_x + A.m_w / 2,
                            .y = A.m_y + A.m_h / 2,
                            .w = A.m_w,
                            .h = A.m_h});

        return TestCase{
            // A=1, B=2
            .inputs = {A, B},
            .expected = {
                Intersection({{.x = B.m_x, .y = B.m_y, .w = A.m_x + A.m_w - B.m_x, .h = A.m_y + A.m_h - B.m_y}}, {1, 2})}};
    }

public:
    static void runAll()
    {
        std::cout << "--> Intersection Tests";
        run(intersection2(), "Intersection of 2 rectangles");
        run(two_non_overlapping_intersections(), "Two non-overlapping intersections");
        run(two_single_overlaps_and_one_triple(), "Two single overlaps and one triple");
        run(adjacent_contained(), "Adjacent and contained");
        run(line_and_corner_dont_intersect(), "Line and corner don't intersect");
        std::cout << "\n";
    }

    // Could generalize this function so it doesn't repeat accross test classes
    static void run(const TestCase &test_case, string name)
    {

        auto actual = Intersection::get_intersections(test_case.inputs);
        print_test_case(actual == test_case.expected, name, [&test_case, &actual]()
                        {
            std::ostringstream os;
            os << "\t expected: " << test_case.expected << "\n\t got: " << actual << "\n";
            return os.str(); });
    }
};

int main()
{
    RectangleTest::runAll();
    IntersectionTest::runAll();
}
