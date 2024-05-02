#include <boost/json.hpp>
#include <functional>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <optional>
#include <ranges>

#include "rectangle.hpp"
#include "intersection.hpp"

using std::string, std::vector;

std::optional<boost::json::value> read_json_from_file(std::string json_string)
{
    // Parse the JSON string
    try  {
        return boost::json::parse(json_string);
    } catch (const std::exception & e) {
        return std::nullopt;
    }
}

std::optional<string> read_to_string(std::string file_path) {
    // Read JSON from file
    std::ifstream ifs(file_path);
    if (!ifs.is_open())
    {
        return std::nullopt;
    }

    std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    // Close the file stream
    ifs.close(); 
    return str;

}
// TODO: parse command line arguments in a nicer way
int main(int argc, char ** argv)
{
    if (argc != 2) {
        std::cout << "Please provide exactly 1 argument: the JSON file name\n";
        return 1;
    }
    auto file_name = std::string(argv[1]);
    auto file_contents = read_to_string(file_name);
    if(!file_contents.has_value()) {
        std::cout << "Error: Could not find file \"" << file_name << "\"\n";
        return 1;
    }

    auto v_opt = read_json_from_file(*file_contents);

    if (!v_opt.has_value()) {
        std::cout << "Improper input: Incorrect JSON syntax\n";
        return 1;
    }

    boost::json::value v = *v_opt;



    if (!v.is_object()) {
        std::cout << "Improper input: top level JSON must be an object\n";
        return 1;
    }

	// will not panic due to check above
    auto obj = v.as_object();

	boost::json::array rects_json;
    try {
        rects_json = obj.at("rects").as_array();
    } catch(const boost::json::error &) {
        std::cout << "Inpropper input: input JSON file must contain \"rects\" field\n";
        return 1;
    }

    if (rects_json.size() < 10) {
        std::cout << "Improper input: \"rects\" field must contain at least 10 rectangles\n";
        return 1;
    }

	vector<Rectangle> rects;
    int count = 0;
    for (auto const & elem : rects_json | std::views::take(10)) {
        auto rect_opt = Rectangle::create(elem);
        if(!rect_opt.has_value()) {
            std::cout << "Improper input: Invalid rectangle element: "  << elem << "\n Rectangles must have exactly for fields: x, y, w, h. All of these should be parsable into uint32_t\n";
            return 1;
        } 
        rects.push_back(*rect_opt);
    }

    std::cout << "Input:\n";
    std::cout << rects;
    std::cout << "Intersections:\n";

    auto result = Intersection::get_intersections(rects);
    std::cout << result;
    return 0;
}