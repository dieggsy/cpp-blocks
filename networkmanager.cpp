#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <vector>

bool string_has_suffix(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size()
        && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void string_truncate(std::string &str,int width) {
    if (str.size() > 10) {
        str.resize(10,' ');
        str.replace(str.end()-1, str.end(), "…");
    }
}

std::string string_join(std::vector<std::string> &strings, std::string sep = "") {
    std::string joined;
    int last = strings.size() - 1;
    for (int i = 0; i < last; ++i) {
        joined += strings[i] + sep;
    }
    joined += strings[last];
    return joined;
};

std::string make_connection_string(sdbus::ObjectPath path) {
    auto active_proxy = sdbus::createObjectProxy
        ("org.freedesktop.NetworkManager", path);
    std::string type = active_proxy->getProperty("Type")
        .onInterface("org.freedesktop.NetworkManager.Connection.Active");
    std::string icon;
    bool only_icon = false;
    if (string_has_suffix(type, "wireless")) {
        icon = "";
    }
    else if (string_has_suffix(type, "ethernet")) {
        icon = "";
    }
    else if (string_has_suffix(type, "vpn")) {
        icon = "";
        only_icon = true;
    }
    else if (string_has_suffix(type, "tun")) {
        icon = "";
        only_icon = true;
    }
    else  { icon = "?"; }
    std::string id = active_proxy->getProperty("Id")
        .onInterface("org.freedesktop.NetworkManager.Connection.Active");
    string_truncate(id,10);
    return icon + (only_icon ? "" : ' ' + id);
}

int main () {
    auto nm_proxy = sdbus::createObjectProxy
        ("org.freedesktop.NetworkManager",
         "/org/freedesktop/NetworkManager");

    auto cons = nm_proxy->getProperty("ActiveConnections")
        .onInterface("org.freedesktop.NetworkManager");
    auto active_connections = cons.get<std::vector<sdbus::ObjectPath>>();
    std::vector<std::string> conn_strings;
    for (const auto conn : active_connections) {
        if (!conn.empty()) {
            conn_strings.push_back(make_connection_string(conn));
        }
    }
    if (conn_strings.empty()) {
        std::cout << " No Connection" << '\n';
    }
    else {
        std::cout << string_join(conn_strings, " ") << '\n';
    }
    return 0;
}
