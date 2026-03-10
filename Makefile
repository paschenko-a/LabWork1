CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror
LDFLAGS = -lgtest -lgtest_main -pthread

TARGETS = bmp test

RESULTS = clockwise.bmp \
          counter_clockwise.bmp \
          gaussian.bmp \
          test_clockwise.bmp \
          test_counter.bmp \
          test_gauss_rotated.bmp \
          test_sample.bmp \
          gaussian_test.bmp

all: $(TARGETS)

bmp: main.cpp bmp_image.cpp bmp_header.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

test: test.cpp bmp_image.cpp bmp_header.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

run: bmp
	./bmp

check: test
	./test

clean:
	rm -f $(TARGETS) $(RESULTS) *.o

.PHONY: all run check clean