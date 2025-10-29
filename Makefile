
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

test-ci: build
	cd build && GTEST_COLOR=1 ctest -V

test-cov: test coverage

coverage:
	@mkdir -p build/coverage
	gcovr --txt \
		--html build/coverage/index.html \
		--html-details \
		--html-theme github.dark-green \
		--html-high-threshold 100 \
		--html-medium-threshold 90 \
		--fail-under-line 100 \
		--fail-under-branch 100 \
		--lcov build/coverage.lcov \
		-e build \
		-e tests

.PHONY: setup build install lint format test test-ci test-cov coverage
