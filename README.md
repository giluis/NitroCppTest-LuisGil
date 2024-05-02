# Luís Gil's submission
date: 20240502

Hi! This  is my submission for the **Nitro C++ programming challenge**.
Took me a long time to submit in part because I was on vacation for some time, and didn't have a lot of free time besides that. I am now satisfied (enough) with the quality of this work. Thank you for this opportunity,  it was very fun and engaging!

## Comments and possible improvements
- Solution is less performant than it could be. However, I wasn't able to find a better balance between performance and readibility, and opted for readibility as the assignment suggested.
- Testing is not complete. I believe I've tested the most important edge cases, but certainly some are missing. I haven't done any testing with 10 rectangles, tho I have run example.json
- Test classes could have been generalised rather than repeated. That would be overengineerd tho 
- Makefiles get the job done, but are archaic and not very maintainable. CMake and Gradle are better but would be over-engineering, in my opinion.
- I came up with the idea for TestCases because they would make it easier to integration test. Just turn the TestCase into a JSON file and run the executable on it. However, I felt like integration-testing was out of scope for this challenge.


## Running

For easy reproducibility, I chose to use Docker, the global standard of containerization.

To read and process a JSON file, build image and run container. If no input file is provided, ./example.json will be used.
Your input file(s) must be present in the current directory before building the image OR you can run the container with a mounted volume. This is necessary since Docker runs a separate file system from the host system.

You can find some JSON examples in ./inputs

```bash
docker build -t <image_name> . && docker run <image_name> <inputfile>
```

If Docker is not an option for you, check Dockerfile for dependencies:
- g++ compiler (v13.12.2). For more info, run `g++ -v` on container
- [libboost 1.70.0](https://packages.ubuntu.com/focal/libboost-all-dev)
- make


The following command wil compile and run the code with `<inputfile>` as input.
```bash
g++ -std=c++20 -I src  -o main src/main.cpp src/rectangle.cpp src/intersection.cpp -lboost_json && ./main <inputfile>
```
note: C++20 is used just for the amazing std::views.

Tests can be run with `make test`. 

For a little more control, feel free to alter the Dockerfile and ssh into the running container or, like me, use VsCode's excellent [Dev Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

