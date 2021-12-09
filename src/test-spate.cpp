#include <testthat.h>
#include "spate.hpp"

context("libspate/grid/seq")
{
    test_that("sequence from 1 to 10 with length 10 is correct")
    {
        vector<double> test_seq = spate::grid::seq(1.0, 10.0, 10);
        vector<double> expected = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

        for (int i = 0; i < 10; ++i) {
          expect_true(test_seq[i] == expected[i]);
        }
    }
}

context("libspate/grid/indexing")
{
    test_that("ensure indexing is correct")
    {
        expect_true( spate::hilbert::pos_index(4, 1, 2) == 7 );
        expect_true( spate::hilbert::pos_index(8, 3, 4) == 31 );
    }
}

context("libspate/grid/positioning")
{
    test_that("ensure positioning is correct")
    {
        size_t test_x[2];
        size_t test_y[2];

        spate::hilbert::index_pos(4, 7, &test_x[0], &test_y[0]);
        spate::hilbert::index_pos(8, 31, &test_x[1], &test_y[1]);

        expect_true(test_x[0] == 1);
        expect_true(test_y[0] == 2);
        expect_true(test_x[1] == 3);
        expect_true(test_y[1] == 4);
    }
}

context("libspate/grid/closest")
{
    test_that("closest() works as expected")
    {
        vector<double> test_vals = {1.0, 5.0, 6.3, 7.2, 10.0};
        expect_true(spate::closest(test_vals, 6) == 2);
        expect_true(spate::closest(test_vals, 5.6) == 1);
        expect_true(spate::closest(test_vals, 7.1) == 3);
        expect_true(spate::closest(test_vals, 8) == 3);
        expect_true(spate::closest(test_vals, 9) == 4); // fails without cast
        expect_true(spate::closest(test_vals, 9.0) == 4);
        expect_true(spate::closest(test_vals, 9.7) == 4);
    }
}

context("libspate/compression")
{
    test_that("compression works as expected")
    {
        vector<size_t> test_vals = {8, 32, 317, 2, 654, 12, 9};
        tuple<vector<size_t>, vector<size_t>> compressed = spate::compress(test_vals);

        vector<size_t> expected_indices = {2, 6, 1, 3, 20, 285, 337};
        vector<size_t> expected_order = {3, 0, 6, 5, 1, 2, 4};

        for (size_t i = 0; i < test_vals.size(); ++i) {
            expect_true(std::get<0>(compressed)[i] == expected_indices[i]);
            expect_true(std::get<1>(compressed)[i] == expected_order[i]);
        }
    }
}

context("libspate/decompression")
{
    test_that("decompression works as expected")
    {
        vector<size_t> test_indices = {2, 6, 1, 3, 20, 285, 337};
        vector<size_t> test_order = {3, 0, 6, 5, 1, 2, 4};
        vector<size_t> expected = {8, 32, 317, 2, 654, 12, 9};
        vector<size_t> decompressed = spate::decompress(test_indices, test_order);

        for (size_t i = 0; i < expected.size(); ++i) {
            expect_true(decompressed[i] == expected[i]);
        }
    }
}