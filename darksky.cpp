#include <sstream>
#include <iostream>
#include <cmath>

#include <rapidjson/document.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

std::string get_icon(std::string icon) {
    if (icon == "clear-day") { return ""; }
    else if (icon == "clear-night") { return ""; }
    else if (icon == "rain") { return ""; }
    else if (icon == "snow") { return ""; }
    else if (icon == "sleet") { return  ""; }
    else if (icon == "wind") { return ""; }
    else if (icon == "fog") {return "";}
    else if (icon == "cloudy") { return ""; }
    else if (icon == "partly-cloudy-day") { return ""; }
    else if (icon == "partly-cloudy-night") { return ""; }
    else if (icon == "hail") { return ""; }
    else if (icon == "thunderstorm") { return ""; }
    else if (icon == "tornado") { return ""; }
    else { return ""; }
}

int main() {
    try {
        // That's all that is needed to do cleanup of used resources (RAII style).
        curlpp::Cleanup myCleanup;

        // Our request to be sent.
        curlpp::Easy myRequest;

        // Set the URL.

        // Send request and get a result.
        // By default the result goes to standard output.
        rapidjson::Document d;
        std::ostringstream os;
        os << curlpp::options::Url("https://api.darksky.net/forecast/"
                                   "00000000000000000000000000000000/"
                                   "0.0,0.0?units=us");
        d.Parse(os.str().c_str());
        float temp = roundf(d["currently"]["temperature"].GetFloat());
        std::string icon_str {d["currently"]["icon"].GetString()};
        std::string icon {get_icon(icon_str)};
        std::cout << icon << ' ' << temp << '\n';
    }

    catch(curlpp::RuntimeError & e) {
        std::cout << e.what() << std::endl;
    }

    catch(curlpp::LogicError & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
