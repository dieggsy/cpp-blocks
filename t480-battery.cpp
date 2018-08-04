#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <cmath>
#include <thread>
#include <chrono>
// #include <boost/algorithm/string/join.hpp>
// #include <boost/algorithm/string/predicate.hpp>

double get_joint_percent(const std::unique_ptr<sdbus::IObjectProxy> &bat0,
                         const std::unique_ptr<sdbus::IObjectProxy> &bat1) {
    double energy0 = bat0->getProperty("Energy")
        .onInterface("org.freedesktop.UPower.Device").get<double>();
    double maxenergy0 = bat0->getProperty("EnergyFull")
        .onInterface("org.freedesktop.UPower.Device").get<double>();
    double energy1 = bat1->getProperty("Energy")
        .onInterface("org.freedesktop.UPower.Device").get<double>();
    double maxenergy1 = bat1->getProperty("EnergyFull")
        .onInterface("org.freedesktop.UPower.Device").get<double>();
    return roundf((energy1 + energy0) / (maxenergy1 + maxenergy0) * 100);
}

std::string get_icon(double percent, bool adapter_online) {
    static int index {-1};
    std::array<std::string, 5> icons {"", "", "", "", ""};
    if (!adapter_online) {
        if (percent > 95) {
            return icons[4];
        }
        else if (percent > 75) {
            return icons[3];
        }
        else if (percent > 50) {
            return icons [2];
        }
        else if (percent > 25) {
            return icons [1];
        }
        else if (percent <= 25) {
            return icons [0];
        }
    }
    else {
        if (percent > 95) {
            return "";
        }
        else if (index == 4) {
            index = 0;
            return icons[0];
        }
        else {
            ++index;
            return icons[index];
        }
    }
    return icons[4];
}

void maybe_warn_low(double percent, bool adapter_online) {
    static int counter {0};
    if (counter == 30 && !adapter_online && percent < 10) {
        system("espeak -vf4 'Battery critically low, consider charging' 2>/dev/null &");
        counter = 1;
    }
    else {++counter;}
}

int main () {
    using namespace std::chrono_literals;
    auto bat0 = sdbus::createObjectProxy
        ("org.freedesktop.UPower",
         "/org/freedesktop/UPower/devices/battery_BAT0");
    auto bat1 = sdbus::createObjectProxy
        ("org.freedesktop.UPower",
         "/org/freedesktop/UPower/devices/battery_BAT1");
    auto adapter = sdbus::createObjectProxy
        ("org.freedesktop.UPower",
         "/org/freedesktop/UPower/devices/line_power_AC");
    // std::cout << get_joint_percent(bat0, bat1) << '\n';
    while (true) {
        double percent = get_joint_percent(bat0,bat1);
        bool adapter_online = adapter->getProperty("Online")
            .onInterface("org.freedesktop.UPower.Device").get<bool>();
        std::cout << get_icon(percent, adapter_online) << ' ' << percent << std::endl;
        maybe_warn_low(percent, adapter_online);
        std::this_thread::sleep_for(1s);
    };
    return 0;
}
