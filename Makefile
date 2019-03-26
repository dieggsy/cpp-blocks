INSTALL_DIR=~/bin/blocks/
CXXFLAGS=-O3

FILES=$(wildcard *.cpp)
TARGETS=$(addprefix build/,$(basename $(FILES)))

all: $(TARGETS)

small: CXXFLAGS += -s
small: all

install:
	mkdir -p $(INSTALL_DIR)
	cp -vr build/* $(INSTALL_DIR)

clean:
	rm -rvf build

build/amixer: CXXFLAGS += -lasound

build/darksky: CXXFLAGS += -lcurlpp -lcurl

build/networkmanager: CXXFLAGS += -lsdbus-c++

build/bluez: CXXFLAGS += -lsdbus-c++ -std=gnu++17

build/t480-battery: CXXFLAGS += -lsdbus-c++

build/% : %.cpp | build # object file depends on its directory
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

build :
	mkdir -p $@
