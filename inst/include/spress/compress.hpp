#pragma once
#ifndef _SPRESS_COMPRESS_H_
#define _SPRESS_COMPRESS_H_

#include <algorithm>
#include <functional>
#include <numeric>
#include <vector>
#include <tuple>

using std::size_t;
using std::vector;
using std::tuple;

namespace spress
{

/**
 * Compress encoded coordinates
 * @param x Encoded/indexed coordinates
 * @return a tuple of the delta compressed coordinates and the original order.
 */
inline tuple<vector<size_t>, vector<size_t>> compress(vector<size_t> x)
{
    size_t xn = x.size();

    vector<size_t> order(xn),
                   new_x(xn);

    // Create and fill permutation vector
    std::iota(order.begin(), order.end(), 0);

    // Sort permutation vector based on `x`
    std::sort(order.begin(), order.end(), [&](size_t i, size_t j) {
        return x[i] < x[j];
    });

    // Create new vector based on ordering
    std::transform(order.begin(), order.end(), new_x.begin(), [&](size_t i) {
        return x[i];
    });
    
    // Get diff
    std::adjacent_difference(new_x.begin(), new_x.end(), new_x.begin());

    return tuple<vector<size_t>, vector<size_t>>{new_x, order};
}

/**
 * Decompress compressed coordinates
 * @param coordinates Delta compressed coordinates
 * @param order Ordering vector
 * @return Original encoded/indexed coordinates.
 */
inline vector<size_t> decompress(vector<size_t> coordinates, vector<size_t> order)
{
    size_t n = coordinates.size();
    vector<size_t> summed(n),
                   sorted(n);

    // Get cumulative sum of encoded vector
    std::partial_sum(
        coordinates.begin(),
        coordinates.end(),
        summed.begin(),
        std::plus<size_t>()
    );

    // Re-sort vector by sort permutation
    for (size_t i = 0; i < n; ++i) {
        sorted[order[i]] = summed[i]; 
    }

    return sorted;
}
}

#endif