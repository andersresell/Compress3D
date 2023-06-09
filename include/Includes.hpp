#pragma once

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <map>
#include <eigen3/Eigen/Dense>
#include <fstream>
#include <set>
#include <utility>
#include <stdint.h>
#include <memory>
#include <any>
#include <chrono>
#include <cfloat>
#include <type_traits>
#include <filesystem>

using std::array;
using std::cerr;
using std::cout;
using std::endl;
using std::make_unique;
using std::map;
using std::max;
using std::min;
using std::move;
using std::pair;
using std::string;
using std::unique_ptr;
namespace filesys = std::filesystem;

using Index = uint32_t;      // Used for accessing cell indices, etc
using ShortIndex = uint16_t; // Used for looping over shorter numbers as spatial dimensions, etc

#ifdef USE_SINGLE_PRECISION
using Scalar = float;
constexpr char Scalar_name[] = "float";
#else
using Scalar = double;
constexpr char Scalar_name[] = "double";
#endif

using Vec3 = Eigen::Vector3<Scalar>;

inline string horizontal_string_Vec3(const Vec3 &v) { return "[" + std::to_string(v[0]) + ", " + std::to_string(v[1]) + ", " + std::to_string(v[2]) + " ]"; }

constexpr ShortIndex N_DIM{3}; // spatial dimensions

// #define FAIL_IF(EXP) ({if (EXP) exit(EXIT_FAILURE); })
#define FAIL_IF_MSG(EXP, MSG) ({if (EXP) {std::cerr << MSG << "\n"; exit(EXIT_FAILURE);} })
#define FAIL_MSG(MSG) ({std::cerr << MSG << "\n"; exit(EXIT_FAILURE); })
#define assert_msg(EXP, MSG) ({if (!EXP) {std::cerr << MSG << "\n"; assert(false);} })

#define DEBUG_LOG_FILE "./debug_log.txt"

using Clock = std::chrono::high_resolution_clock;
using Time = std::chrono::_V2::system_clock::time_point;
using Seconds = std::chrono::seconds;
using Milliseconds = std::chrono::milliseconds;
using std::chrono::duration_cast;

/*Adding range checking to the [] operator for std::vector in debug mode (NDEBUG not defined)*/
template <typename T>
class Vector final : public std::vector<T>
{
public:
    using std::vector<T>::vector;
#ifndef NDEBUG

    T &operator[](size_t i)
    {
        assert(i < this->size());
        return *(this->_M_impl._M_start + i);
    }

    const T &operator[](size_t i) const
    {
        assert(i < this->size());
        return *(this->_M_impl._M_start + i);
    }
#endif
};

template <typename T, size_t N>
inline bool arrays_equal(const array<T, N> &a, const array<T, N> &b)
{
    for (size_t i{0}; i < N; i++)
        if (a[i] != b[i])
            return false;
    return true;
}

template <typename T>
inline int sign(T val)
{
    return val >= 0;
}

template <typename EigenTypeOrScalar>
inline bool is_approx_equal(EigenTypeOrScalar a, EigenTypeOrScalar b)
{
    constexpr Scalar EPS = 1e-8;
    if constexpr (std::is_scalar<EigenTypeOrScalar>())
        return abs(a - b) < EPS;
    else
        return a.isApprox(b, EPS);
}

inline bool is_approx_zero(Scalar val)
{
    return is_approx_equal(val, 0.0);
}

template <typename T>
inline bool num_is_valid(T val)
{
    static_assert(std::is_same<T, Scalar>());

    if (std::isnan(val) || !std::isfinite(val))
        return false;
    return true;
}

template <typename T>
inline bool num_is_valid_and_pos(T val)
{
    if (num_is_valid(val) && val > 0.0)
        return true;
    return false;
}
