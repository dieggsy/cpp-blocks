CXXFLAGS=-O3

all: $(addprefix build/,amixer darksky intel-backlight networkmanager bluez t480-battery)

install:
	mkdir -p ~/bin/blocks
	cp -vr build/* ~/bin/blocks/

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
