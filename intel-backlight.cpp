#include <iostream>
#include <fstream>
#include <cmath>

int main () {
    float curr;
    std::ifstream fbright{"/sys/class/backlight/intel_backlight/brightness"};
    fbright >> curr;

    float max;
    std::ifstream fmax{"/sys/class/backlight/intel_backlight/max_brightness"};
    fmax >> max;
    std::cout << round((curr/max) * 100) << '\n';
    return 0;
}
