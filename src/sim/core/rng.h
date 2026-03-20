#pragma once
#include <cstdint>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

inline uint64_t next_seed(uint64_t s) {
    s ^= s << 13;
    s ^= s >> 7;
    s ^= s << 17;
    return s;
}

inline double uniform_01(uint64_t& s) {
    s = next_seed(s);
    return static_cast<double>(s & 0xFFFFFFFF) / 4294967296.0;
}

inline double normal_dist(double mean, double stddev, uint64_t& s) {
    double u1 = uniform_01(s);
    double u2 = uniform_01(s);
    if (u1 < 1e-15) u1 = 1e-15;
    double z = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
    return mean + stddev * z;
}

inline double lognormal_dist(double median, double sigma, uint64_t& s) {
    double mu = std::log(median);
    double z  = normal_dist(0.0, 1.0, s);
    return std::exp(mu + sigma * z);
}
