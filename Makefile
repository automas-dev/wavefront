
CONFIG=Debug

all: setup build

setup:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=${CONFIG}

build:
	cmake --build build --config ${CONFIG}

install:
	cmake --install build --config ${CONFIG}

lint:
	@find src include tests -name '*.cpp' -or -name '*.hpp' | xargs clang-format --dry-run --Werror --sort-includes

format:
	@find src include tests -name '*.cpp' -or -name '*.hpp' | xargs clang-format -i --Werror --sort-includes

test: build
	cd build && GTEST_COLOR=1 ctest --output-on-failure

.PHONY: setup build install lint format test
