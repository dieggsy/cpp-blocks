#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>

std::string make_connection_string(sdbus::ObjectPath path) {
    auto active_proxy = sdbus::createObjectProxy
        ("org.freedesktop.NetworkManager", path);
    std::string type = active_proxy->getProperty("Type")
        .onInterface("org.freedesktop.NetworkManager.Connection.Active");
    std::string icon;
    if (boost::algorithm::ends_with(type, "wireless")) {
        icon = "";
    }
    else if (boost::algorithm::ends_with(type, "ethernet")) {
        icon = "<>";
    }
    else  { icon = "?"; }
    std::string id = active_proxy->getProperty("Id")
        .onInterface("org.freedesktop.NetworkManager.Connection.Active");
    return icon + ' ' + id;
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
        std::cout << "  No Connection" << '\n';
    }
    else {
        std::cout << boost::algorithm::join(conn_strings, " ") << '\n';
    }
    return 0;
}
