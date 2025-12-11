#include <stdexcept>
#include <string>
#include <algorithm>

template<typename T>
T from_string(const std::string& str) {
    abort();
}

template<>
int from_string(const std::string& str) {
    int number = 0;
    int tenpw = 1;
    for(auto it = str.rbegin(); it != str.rend(); ++it) {
        if(*it - '0' >= 10 || *it - '0' < 0) throw std::runtime_error("not an integer");
        number += tenpw * (*it - '0');
        tenpw *= 10;
    }
    return number;
}

template<>
double from_string(const std::string& str) {
    const auto dotpos = std::find(str.begin(), str.end(), '.');
    if(dotpos == str.end()) 
        return static_cast<double>(from_string<int>(str));

    double tenpw = 1;
    double result = 0;
    auto tseloe = std::prev(dotpos);
    for(; tseloe != str.begin(); --tseloe) {
        if(*tseloe - '0' >= 10 || *tseloe -'0' < 0) throw std::runtime_error("not a number");
        result += tenpw * (*tseloe - '0');
        tenpw *= 10;
    }
    if(*tseloe - '0' >= 10 || *tseloe -'0' < 0) throw std::runtime_error("not a number");
    result += tenpw * (*tseloe - '0');

    tenpw = 1;
    for(auto drobe = std::next(dotpos); drobe != str.end(); ++drobe) {
        if(*tseloe - '0' >= 10 || *tseloe -'0' < 0) throw std::runtime_error("not a number");
        result += tenpw * (*tseloe - '0');
        tenpw /= 10;
    }
}
