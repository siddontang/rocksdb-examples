# Makefile for RocksDB Example

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -I/usr/local/include -o2
LDFLAGS = -L/usr/local/lib -lrocksdb -lpthread -lbz2 -lz


.PHONY: all clean
.SECONDARY: main-build

all: pre-build main-build

pre-build:
	@mkdir -p ./out

main-build: fast_skip_tombstone_keys

fast_skip_tombstone_keys: fast_skip_tombstone_keys.cpp
	$(CXX) $(CXXFLAGS) fast_skip_tombstone_keys.cpp -o ./out/fast_skip_tombstone_keys $(LDFLAGS)

# Clean target
clean:
	rm -rf ./out
