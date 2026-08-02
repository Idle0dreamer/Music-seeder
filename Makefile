CXX ?= g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra -Wpedantic -Werror -Iinclude
DBGFLAGS := -std=c++23 -O0 -g3 -Wall -Wextra -Wpedantic -Werror -Iinclude

KERNEL_SRC := $(shell find src/kernel -name '*.cpp' | sort)
KERNEL_TEST := $(shell find tests/kernel -name '*.cpp' | sort)
KERNEL_APP_SRC := $(shell find apps/kernel -name '*.cpp' | sort)
KERNEL_APP := $(KERNEL_SRC) $(KERNEL_APP_SRC)
SYNTH_SRC := $(KERNEL_SRC) $(shell find src/synthesis -name '*.cpp' ! -name 'FaustRender.cpp' | sort)
SYNTH_APP_SRC := $(shell find apps/synthesis -name '*.cpp' ! -name 'faust_main.cpp' | sort)
SYNTH_OBJ := $(patsubst %.cpp,build/synthesis/%.o,$(SYNTH_SRC) $(SYNTH_APP_SRC))
FAUST_ROOT := third_party/audio/faust
FAUST_LIBRARIES := third_party/audio/faustlibraries
FAUST_ARCHITECTURE := $(FAUST_ROOT)/architecture/minimal-effect.cpp
FAUST_COMPILER := $(FAUST_ROOT)/build/bin/faust
FAUST_GENERATED := build/generated/music_seed_santur_courses.cpp
FAUST_SRC := $(KERNEL_SRC) $(shell find src/synthesis -name '*.cpp' ! -name 'FaustRender.cpp' | sort) src/synthesis/FaustRender.cpp
FAUST_APP_SRC := apps/synthesis/faust_main.cpp
FAUST_OBJ := $(patsubst %.cpp,build/faust/%.o,$(FAUST_SRC) $(FAUST_APP_SRC))
FAUST_CXXFLAGS := $(CXXFLAGS) -Wno-unused-parameter -Wno-attributes -I$(FAUST_ROOT)/architecture -Ibuild/generated
TEST_SRC := $(KERNEL_SRC) $(KERNEL_TEST)
TEST_OBJ := $(patsubst %.cpp,build/debug/%.o,$(TEST_SRC))
APP_OBJ := $(patsubst %.cpp,build/release/%.o,$(KERNEL_APP))
UB_OBJ := $(patsubst %.cpp,build/undefined/%.o,$(TEST_SRC))
AS_OBJ := $(patsubst %.cpp,build/address/%.o,$(TEST_SRC))
DEPS := $(TEST_OBJ:.o=.d) $(APP_OBJ:.o=.d) \
	$(UB_OBJ:.o=.d) $(AS_OBJ:.o=.d) $(SYNTH_OBJ:.o=.d)

.PHONY: all test kernel synthesis synthesis-faust faust-compiler kernel-test kernel-sanitize kernel-address clean

all: kernel

test: kernel-test

kernel: build/kernel

synthesis: build/synthesis-render

synthesis-faust: build/synthesis-faust-render

build/synthesis-render: $(SYNTH_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

build/synthesis-faust-render: $(FAUST_OBJ)
	$(CXX) $(FAUST_CXXFLAGS) $^ -o $@

faust-compiler:
	$(MAKE) -C $(FAUST_ROOT)/build CMAKE=cmake cmake \
		BACKENDS=regular.cmake TARGETS=regular.cmake
	$(MAKE) -C $(FAUST_ROOT)/build CMAKE=cmake -j2

$(FAUST_GENERATED): synthesis/models/santur_courses.dsp faust-compiler
	@mkdir -p $(dir $@)
	$(FAUST_COMPILER) -I $(FAUST_LIBRARIES) -lang cpp \
		-a $(FAUST_ARCHITECTURE) \
		-cn music_seed_santur_courses $< -o $@

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

build/synthesis/%.o: %.cpp Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

build/faust/src/synthesis/FaustRender.o: src/synthesis/FaustRender.cpp \
		Makefile $(FAUST_GENERATED)
	@mkdir -p $(dir $@)
	$(CXX) $(FAUST_CXXFLAGS) -MMD -MP -c $< -o $@

build/faust/%.o: %.cpp Makefile $(FAUST_GENERATED)
	@mkdir -p $(dir $@)
	$(CXX) $(FAUST_CXXFLAGS) -MMD -MP -c $< -o $@

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
