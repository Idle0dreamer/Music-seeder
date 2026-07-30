CXX ?= g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra -Wpedantic -Werror -Iinclude
DBGFLAGS := -std=c++23 -O0 -g3 -Wall -Wextra -Wpedantic -Werror -Iinclude

KERNEL_SRC := $(shell find src/kernel -name '*.cpp' | sort)
KERNEL_TEST := $(shell find tests/kernel -name '*.cpp' | sort)
KERNEL_APP := $(KERNEL_SRC) apps/kernel/main.cpp
TEST_SRC := $(KERNEL_SRC) $(KERNEL_TEST)
TEST_OBJ := $(patsubst %.cpp,build/debug/%.o,$(TEST_SRC))
APP_OBJ := $(patsubst %.cpp,build/release/%.o,$(KERNEL_APP))
UB_OBJ := $(patsubst %.cpp,build/undefined/%.o,$(TEST_SRC))
AS_OBJ := $(patsubst %.cpp,build/address/%.o,$(TEST_SRC))
DEPS := $(TEST_OBJ:.o=.d) $(APP_OBJ:.o=.d) \
	$(UB_OBJ:.o=.d) $(AS_OBJ:.o=.d)

.PHONY: all test kernel kernel-test kernel-sanitize kernel-address clean

all: kernel

test: kernel-test

kernel: build/kernel

build/kernel: $(APP_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/kernel-tests: $(TEST_OBJ)
	$(CXX) $(DBGFLAGS) $^ -o $@

build/debug/%.o: %.cpp Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(DBGFLAGS) -MMD -MP -c $< -o $@

build/release/%.o: %.cpp Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

kernel-test: build/kernel-tests
	./build/kernel-tests

kernel-sanitize: build/kernel-tests-sanitize
	./build/kernel-tests-sanitize

kernel-address: build/kernel-tests-address
	ASAN_OPTIONS=detect_leaks=0 ./build/kernel-tests-address

build/kernel-tests-sanitize: $(UB_OBJ)
	$(CXX) $(DBGFLAGS) -fsanitize=undefined \
		-fno-omit-frame-pointer $^ -o $@

build/kernel-tests-address: $(AS_OBJ)
	$(CXX) $(DBGFLAGS) -fsanitize=address \
		-fno-omit-frame-pointer $^ -o $@

build/undefined/%.o: %.cpp Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(DBGFLAGS) -fsanitize=undefined \
		-fno-omit-frame-pointer -MMD -MP -c $< -o $@

build/address/%.o: %.cpp Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(DBGFLAGS) -fsanitize=address \
		-fno-omit-frame-pointer -MMD -MP -c $< -o $@

clean:
	@find build -mindepth 1 -delete 2>/dev/null || true

-include $(DEPS)
