build ?= build

out := $(patsubst src/%, $(build)/bin/%.o, $(wildcard src/*.cpp))

dir := $(build)/bin $(build)/out

.PHONY: clean all

all: $(dir) $(build)/bin/game

$(dir):
	mkdir -p $@

clean:
	$(RM) -r $(build)

$(build)/bin/game: $(out)
	$(CXX) -o $@ $^ -std=gnu++17 -Wall -Wextra -pedantic -lraylib

$(out): $(build)/bin/%.o: src/%
	$(CXX) -c $^ -o $@ -std=gnu++17 -Wall -Wextra -pedantic

