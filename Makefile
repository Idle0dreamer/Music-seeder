CXX ?= g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra -Wpedantic -Werror -Iinclude
DBGFLAGS := -std=c++23 -O0 -g3 -Wall -Wextra -Wpedantic -Werror -Iinclude

SRC := $(shell find src -name '*.cpp' ! -path 'src/app/main.cpp' | sort)
APP := $(SRC) src/app/main.cpp
TEST := $(SRC) tests/main.cpp
KERNEL_SRC := $(shell find src/kernel -name '*.cpp' | sort)
KERNEL_TEST := $(shell find tests/kernel -name '*.cpp' | sort)
KERNEL_APP := $(KERNEL_SRC) apps/kernel/main.cpp

.PHONY: all test kernel kernel-test kernel-sanitize kernel-address clean debug

all: build/maqam

build/maqam: $(APP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(APP) -o $@

build/tests: $(TEST)
	@mkdir -p build
	$(CXX) $(DBGFLAGS) $(TEST) -o $@

test: build/tests
	./build/tests

kernel: build/kernel

build/kernel: $(KERNEL_APP)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(KERNEL_APP) -o $@

build/kernel-tests: $(KERNEL_SRC) $(KERNEL_TEST)
	@mkdir -p build
	$(CXX) $(DBGFLAGS) $(KERNEL_SRC) $(KERNEL_TEST) -o $@

kernel-test: build/kernel-tests
	./build/kernel-tests

kernel-sanitize:
	@mkdir -p build
	$(CXX) $(DBGFLAGS) -fsanitize=undefined \
		-fno-omit-frame-pointer $(KERNEL_SRC) $(KERNEL_TEST) \
		-o build/kernel-tests-sanitize
	./build/kernel-tests-sanitize

kernel-address:
	@mkdir -p build
	$(CXX) $(DBGFLAGS) -fsanitize=address \
		-fno-omit-frame-pointer $(KERNEL_SRC) $(KERNEL_TEST) \
		-o build/kernel-tests-address
	ASAN_OPTIONS=detect_leaks=0 ./build/kernel-tests-address

debug:
	@mkdir -p build
	$(CXX) $(DBGFLAGS) $(APP) -o build/maqam

clean:
	@find build -mindepth 1 -maxdepth 1 -type f -delete 2>/dev/null || true
