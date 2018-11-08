#include <iostream>
#include <sdbus-c++/sdbus-c++.h>
#include <map>
#include <string>

int main () {
    auto bluez_proxy = sdbus::createObjectProxy("org.bluez", "/");
    std::map<sdbus::ObjectPath,
             std::map<std::string,
                      std::map<std::string,
                               sdbus::Variant>>> result;
    bluez_proxy->callMethod("GetManagedObjects")
        .onInterface("org.freedesktop.DBus.ObjectManager")
        .storeResultsTo(result);
    if (result.size() > 1) {
        std::cout << "";
        for (auto& [key,val] : result) {
            if (!(val.find("org.bluez.Device1") == val.end())) {
                if (val["org.bluez.Device1"]["Connected"].get<bool>()) {
                    std::cout << ' ' << val["org.bluez.Device1"]["Name"]
                        .get<std::string>();
                    break;
                }
            }
        }
        std::cout << "\n";
    }
    return 0;
}
