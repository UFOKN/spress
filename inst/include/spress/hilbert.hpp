#pragma once
#ifndef _SPRESS_HILBERT_H_
#define _SPRESS_HILBERT_H_

#include <numeric>
using std::size_t;

namespace spress
{
namespace hilbert
{
/**
 * Rotate X/Y coordinates based on Hilbert Curve.
 * @param n Dimensions of n x n grid. Must be a power of 2.
 * @param x [out] Pointer to the X coordinate variable
 * @param y [out] Pointer to the Y coordinate variable
 * @param rx Numeric bool for determining if rotation is needed
 * @param ry Numeric bool for determining if rotation is needed
 */
inline void rotate(size_t n, size_t *x, size_t *y, size_t rx, size_t ry)
{
    if (ry == 0) {
        if (rx == 1) {
            *x = n - 1LL - *x;
            *y = n - 1LL - *y;
        }

        size_t t = *x;
        *x = *y;
        *y = t;
    }
}

/**
 * Convert a grid position to a Hilbert Curve index.
 * @param n Dimensions of n x n grid. Must be a power of 2.
 * @param x X coordinate in grid form
 * @param y Y coordinate in grid form
 */
inline size_t pos_index(size_t n, size_t x, size_t y)
{
    size_t rx, ry;
    size_t d = 0LL;

    for (size_t s = n / 2LL; s > 0LL; s /= 2LL) {
        rx = (x & s) > 0LL;
        ry = (y & s) > 0LL;
        d += s * s * ((3LL * rx) ^ ry);
        rotate(n, &x, &y, rx, ry);
    }

    return d;
}

/**
 * Convert a Hilbert Curve index to a grid position.
 * @param n Dimensions of n x n grid. Must be a power of 2.
 * @param d Hilbert Curve index/distance.
 * @param x [out] Pointer to X coordinate variable
 * @param y [out] Pointer to Y coordinate variable
 */
inline void index_pos(size_t n, size_t d, size_t *x, size_t *y)
{
    size_t rx, ry;
    size_t t = d;
    *x = 0LL;
    *y = 0LL;

    for (size_t s = 1LL; s < n; s *= 2LL) {
        rx = 1LL & (t / 2LL);
        ry = 1LL & (t ^ rx);
        rotate(s, x, y, rx, ry);

        *x += s * rx;
        *y += s * ry;
        
        t /= 4LL;
    }
}
}
}

#endif