#pragma once

#include <chrono>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <string>

namespace ch = std::chrono;

using TimeT = ch::duration<long, std::nano>;

class Benchmark {
    static inline decltype(auto) now() {
        return ch::high_resolution_clock::now();
    }

public:
    struct Timestamp {
        TimeT duration{0};

        Timestamp() = default;

        template <typename Rep, typename Period>
        Timestamp(ch::duration<Rep, Period> time)
            : duration{ch::duration_cast<TimeT>(time)} {}

        Timestamp& operator+=(const Timestamp& other) {
            duration += other.duration;
            return *this;
        }

        Timestamp operator+(const Timestamp& other) const {
            Timestamp result = *this;
            result += other;
            return result;
        }

        Timestamp& operator/=(long long value) {
            duration /= value;
            return *this;
        }

        Timestamp operator/(long long value) const {
            Timestamp result = *this;
            result /= value;
            return result;
        }

        Timestamp operator/(std::size_t value) const {
            return *this / static_cast<long long>(value);
        }
    };

    template <typename FuncT>
    static inline Timestamp measure(FuncT&& func) {
        auto start = now();
        func();
        auto end = now();

        return Timestamp{end - start};
    }
};

inline std::ostream& operator<<(std::ostream& os, const Benchmark::Timestamp& timestamp) {
    auto duration = timestamp.duration;
    std::ostringstream result;
    int precision = 1;

    auto append_part = [&](auto unit, const char* suffix) -> bool {
        using UnitT = decltype(unit);

        if (auto converted = ch::duration_cast<UnitT>(duration); converted.count()) {
            result << converted.count() << suffix;
            --precision;
            duration -= ch::duration_cast<TimeT>(converted);

            if (!precision) {
                return true;
            }
        }

        return !precision;
    };

    if (!(append_part(ch::hours{}, "h") ||
          append_part(ch::minutes{}, "min") ||
          append_part(ch::seconds{}, "s") ||
          append_part(ch::milliseconds{}, "ms") ||
          append_part(ch::microseconds{}, "us") ||
          append_part(ch::nanoseconds{}, "ns"))) {
        result << "0ns";
    }

    os << result.str();
    return os;
}
