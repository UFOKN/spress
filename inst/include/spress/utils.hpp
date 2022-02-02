#pragma once
#ifndef _SPRESS_UTILS_H_
#define _SPRESS_UTILS_H_

#include <vector>
#include <algorithm>
#include <numeric>
#include "grid.hpp"

using std::abs;
using std::size_t;
using std::vector;

namespace spress
{
namespace utils
{

/**
 * Get the index of the closest value in a vector from some other value.
 * @param v Vector to compare value to
 * @param value Value to compare vector elements to
 */
template <typename T, typename S>
inline size_t closest(vector<T>& v, const S& value)
{
    // Setup
    typename vector<T>::iterator first = v.begin();
    typename vector<T>::iterator last  = v.end();
    // T & S should always be of integer or floating point type.
    // So, in theory this should be safe.
    T comp = static_cast<T>(value);

    // Extended Bounds
    typename vector<T>::iterator lb = std::lower_bound(first, last, comp) - 1;
    typename vector<T>::iterator up = std::upper_bound(first, last, comp) + 1;

    // Find closest index
    typename vector<T>::iterator index = lb;
    T min = abs(*lb - comp);
    for (typename vector<T>::iterator it = lb + 1; it != up; ++it) {
        T tmp = abs(*it - comp);
        if (tmp < min) {
            min   = tmp;
            index = it;
        }
    }
    
    return std::distance(first, index);
}

/**
 * Get the grid index of values in an n-length grid
 * @param n Grid division amount
 * @param v Vector of values to index
 */
template <typename T>
inline vector<size_t> index(size_t n, vector<T> v)
{
    size_t vlen = v.size();
    vector<size_t> indices(vlen);
    auto mm = std::minmax_element(v.begin(), v.end());
    vector<T> vseq = grid::seq(*mm.first, *mm.second, n);

    for (size_t i = 0; i < vlen; ++i) {
        indices[i] = closest(vseq, v[i]);
    }

    return indices;
}

}
}

#endif