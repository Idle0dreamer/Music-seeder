CXX ?= g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra -Wpedantic -Werror -Iinclude
DBGFLAGS := -std=c++23 -O0 -g3 -Wall -Wextra -Wpedantic -Werror -Iinclude

KERNEL_SRC := $(shell find src/kernel -name '*.cpp' | sort)
KERNEL_TEST := $(shell find tests/kernel -name '*.cpp' | sort)
KERNEL_APP := $(KERNEL_SRC) apps/kernel/main.cpp
KERNEL_HDR := $(shell find include/mq/kernel -name '*.hpp' | sort)
TEST_HDR := $(shell find tests/kernel -name '*.hpp' | sort)

.PHONY: all test kernel kernel-test kernel-sanitize kernel-address clean

all: kernel

test: kernel-test

kernel: build/kernel

build/kernel: Makefile $(KERNEL_APP) $(KERNEL_HDR)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(KERNEL_APP) -o $@

build/kernel-tests: Makefile $(KERNEL_SRC) $(KERNEL_TEST) $(KERNEL_HDR) $(TEST_HDR)
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

clean:
	@find build -mindepth 1 -maxdepth 1 -type f -delete 2>/dev/null || true
