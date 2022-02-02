#pragma once
#ifndef _SPRESS_GRID_H_
#define _SPRESS_GRID_H_

#include <numeric>
#include <vector>
#include <tuple>
#include "hilbert.hpp"

using std::size_t;
using std::vector;
using std::tuple;

namespace spress
{
namespace grid
{
/**
 * Create a linearly spaced vector.
 * @param from Starting value
 * @param to Ending value
 * @param n Size of vector
 * @return Vector of size n with equally spaced values
 */
template <typename T>
inline vector<T> seq(T from, T to, size_t n)
{
    T x;
    T diff = (to - from) / static_cast<T>(n - 1);
    vector<T> s(n);
    typename vector<T>::iterator i;
    
    for (i = s.begin(), x = from; i != s.end(); ++i, x += diff) {
        *i = x;
    }

    return s;
}

/**
 * Create a Hilbert Curve grid array.
 * @param n Dimensions of n x n grid. Must be a power of 2.
 * @param xmax Bounding box value
 * @param xmin Bounding box value
 * @param ymax Bounding box value
 * @param ymin Bounding box value
 */
template <typename T>
inline tuple<vector<T>, vector<T>, vector<size_t>, vector<size_t>, vector<size_t>> grid(size_t n, T xmax, T xmin, T ymax, T ymin)
{
    vector<T> xseq = seq(xmin, xmax, n);
    vector<T> yseq = seq(ymin, ymax, n);

    size_t tot = n * n;

    vector<T> gridX(tot),
              gridY(tot);

    vector<size_t> posX(tot),
                   posY(tot),
                   index(tot);

    size_t j = -1LL;
    for (size_t k = 0LL; k < tot; ++k) {
        size_t i = k % n;
        if (i == 0LL) j++;

        gridX[k] = xseq[i];
        posX[k]  = i;
        gridY[k] = yseq[j];
        posY[k]  = j;
        index[k] = hilbert::pos_index(n, i, j);
    }

    return tuple<vector<T>, vector<T>, vector<size_t>, vector<size_t>, vector<size_t>>
    {
        gridX, gridY,
        posX, posY,
        index
    };
}
}
}

#endif