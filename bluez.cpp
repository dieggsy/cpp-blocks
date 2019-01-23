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
    std::string icon = "";
    std::string devname = "";
    if (result.size() > 1) {
        icon =  "";
        for (auto& [key,val] : result) {
            if (!(val.find("org.bluez.Device1") == val.end())) {
                if (val["org.bluez.Device1"]["Connected"].get<bool>()) {
                    icon = "";
                    devname = " %{F#665C54}"+ val["org.bluez.Device1"]["Name"]
                        .get<std::string>() + "%{F-}";
                    break;
                }
            }
        }
    }
    std::cout << icon << devname << "\n";
    return 0;
}
