CXX := g++ 
CXXFLAGS := -std=c++20 -I src
TARGET := main
SOURCES := src/main.cpp src/rectangle.cpp src/intersection.cpp
LIBS := -lboost_json 
TEST_SOURCES := src/tests.cpp src/rectangle.cpp src/intersection.cpp
TEST_TARGET := tests


$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS)  -o $(TARGET) $(SOURCES) $(LIBS)

run: $(TARGEt)
	./$(TARGET) $(ARGS)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES) $(LIBS)

test: $(TEST_TARGET)
	./$(TEST_TARGET) 

clean:
	rm -f $(TEST_TARGET) $(TARGET)
