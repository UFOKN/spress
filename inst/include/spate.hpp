#pragma once
#ifndef SPATE_H_
#define SPATE_H_

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

using std::size_t;
using std::vector;
using std::tuple;
using std::string;

namespace spate
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

    namespace grid
    {
        /**
         * Create a linearly spaced vector.
         * @param from Starting value
         * @param to Ending value
         * @param n Size of vector
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
            vector<T> gridX(tot), gridY(tot);
            vector<size_t> posX(tot), posY(tot), index(tot);

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
                gridX, gridY, posX, posY, index
            };
        }
    }

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
        T min = std::abs(*lb - comp);
        for (typename vector<T>::iterator it = lb + 1; it != up; ++it) {
            T tmp = std::abs(*it - comp);
            if (tmp < min) {
                min   = tmp;
                index = it;
            }
        }
        
        return std::distance(first, index);
    }

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

    template <typename T>
    inline vector<size_t> encode(size_t n, vector<T> x, vector<T> y)
    {
        size_t gridN = x.size();
        vector<size_t> gridX = index(n, x);
        vector<size_t> gridY = index(n, y);
        vector<size_t> gridH(gridN);

        for (size_t i = 0; i < gridN; ++i) {
            gridH[i] = hilbert::pos_index(n, gridX[i], gridY[i]);
        }

        return gridH;
    }

    template <typename T>
    inline tuple<vector<T>, vector<T>> decode(size_t n, vector<size_t> h, T xmax, T xmin, T ymax, T ymin)
    {
        size_t gridN = h.size();
        vector<size_t> gridX(gridN);
        vector<size_t> gridY(gridN);
        vector<T> x(gridN);
        vector<T> y(gridN);
        vector<T> xspace = grid::seq(xmin, xmax, n);
        vector<T> yspace = grid::seq(ymin, ymax, n);

        for (size_t i = 0; i < gridN; ++i) {
            hilbert::index_pos(n, h[i], &gridX[i], &gridY[i]);

            x[i] = xspace[gridX[i]];
            y[i] = yspace[gridY[i]];
        }

        return tuple<vector<T>, vector<T>>{x, y};
    }
}

#endif