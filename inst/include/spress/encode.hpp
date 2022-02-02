#pragma once
#ifndef _SPRESS_ENCODE_H_
#define _SPRESS_ENCODE_H_

#include <numeric>
#include <vector>
#include <tuple>
#include "hilbert.hpp"
#include "grid.hpp"
#include "utils.hpp"

using std::size_t;
using std::vector;
using std::tuple;

namespace spress
{

/**
 * Encode a set of coordinates
 * @param n Number of grid rows/cols
 * @param x X coordinates (i.e. Longitude)
 * @param y Y coordinates (i.e. Latitude)
 * @return A `std::vector<std::size_t>` `H` of indexed coordinates
 *         with the same length as `x` and `y`.
 */
template <typename T>
inline vector<size_t> encode(size_t n, vector<T> x, vector<T> y)
{
    size_t gridN = x.size();
    vector<size_t> gridX = utils::index(n, x);
    vector<size_t> gridY = utils::index(n, y);
    vector<size_t> gridH(gridN);

    for (size_t i = 0; i < gridN; ++i) {
        gridH[i] = hilbert::pos_index(n, gridX[i], gridY[i]);
    }

    return gridH;
}

/**
 * Decode a set of indexed coordinates
 * @param n Number of grid rows/cols used
 * @param h Integer vector contained indexed coordinates
 * @param xmax Maximum X coordinate used for encoding
 * @param xmin Minimum X coordinate used for encoding
 * @param ymax Maximum Y coordinate used for encoding
 * @param ymin Minimum Y coordinate used for encoding
 * @return `std::tuple` with X and Y coordinate vectors as elements.
 */
template <typename T>
inline tuple<vector<T>, vector<T>> decode(size_t n, vector<size_t> h, T xmax, T xmin, T ymax, T ymin)
{
    size_t gridN = h.size();

    vector<size_t> gridX(gridN),
                   gridY(gridN);

    vector<T> x(gridN),
              y(gridN);

    // Get sequences of the coordinate space
    // that the indexed coordinates lie in
    vector<T> xspace = grid::seq(xmin, xmax, n);
    vector<T> yspace = grid::seq(ymin, ymax, n);

    for (size_t i = 0; i < gridN; ++i) {
        // Index grid position and append grid coordinates to x/y
        hilbert::index_pos(n, h[i], &gridX[i], &gridY[i]);
        x[i] = xspace[gridX[i]];
        y[i] = yspace[gridY[i]];
    }

    return tuple<vector<T>, vector<T>>{x, y};
}

}

#endif