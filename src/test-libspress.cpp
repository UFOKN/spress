#include <testthat.h>
#include "spress.hpp"
#include <iostream>

context("libspress/grid")
{
    test_that("sequence from 1 to 10 with length 10 is correct")
    {
        vector<double> test_seq = spress::grid::seq(1.0, 10.0, 10),
                       expected = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

        for (int i = 0; i < 10; ++i) {
          expect_true(test_seq[i] == expected[i]);
        }
    }
}

context("libspress/hilbert")
{
    test_that("ensure indexing is correct")
    {
        expect_true( spress::hilbert::pos_index(4, 1, 2) == 7 );
        expect_true( spress::hilbert::pos_index(8, 3, 4) == 31 );
    }

    test_that("ensure positioning is correct")
    {
        size_t test_x[2];
        size_t test_y[2];

        spress::hilbert::index_pos(4, 7, &test_x[0], &test_y[0]);
        spress::hilbert::index_pos(8, 31, &test_x[1], &test_y[1]);

        expect_true(test_x[0] == 1);
        expect_true(test_y[0] == 2);
        expect_true(test_x[1] == 3);
        expect_true(test_y[1] == 4);
    }
}

context("libspress/utils")
{
    test_that("closest() works as expected")
    {
        vector<double> test_vals = {1.0, 5.0, 6.3, 7.2, 10.0};
        expect_true(spress::utils::closest(test_vals, 6) == 2);
        expect_true(spress::utils::closest(test_vals, 5.6) == 1);
        expect_true(spress::utils::closest(test_vals, 7.1) == 3);
        expect_true(spress::utils::closest(test_vals, 8) == 3);
        expect_true(spress::utils::closest(test_vals, 9) == 4); // fails without cast
        expect_true(spress::utils::closest(test_vals, 9.0) == 4);
        expect_true(spress::utils::closest(test_vals, 9.7) == 4);
    }
}

context("libspress/encode")
{
    test_that("encoding works as expected for POINT geometry")
    {
        vector<double> x = {-79.1293835987096, -77.9607369942917, -77.8885148336456, -77.9695196161608, -77.9673753, -77.9680932},
                       y = {35.5408023979107, 34.0262861497925, 34.5153732259362, 34.2355059508518, 34.2788964278153, 34.2789019278153};
        
        vector<size_t> expected1 = {21, 63, 49, 62, 62, 62},
                       expected2 = {21845, 65280, 50417, 63579, 63642, 63642},
                       test_vals1 = spress::encode(8, x, y),
                       test_vals2 = spress::encode(256, x, y);

        for (size_t i = 0; i < test_vals1.size(); ++i) {
            expect_true(test_vals1[i] == expected1[i]);
            expect_true(test_vals2[i] == expected2[i]);
        }
    }

    test_that("decoding works as expected for POINT indexes")
    {   
        float eps = 0.000001;
        vector<double> bb = {-77.8885148336456, -79.1293835987096, 35.5408023979107, 34.0262861497925};
        vector<size_t> h1 = {21, 63, 49, 62, 62, 62},
                       h2 = {21845, 65280, 50417, 63579, 63642, 63642};

        // Some coordinates
        vector<double> expected_x1 = {-79.12938, -77.88851, -77.88851, -77.88851, -77.88851, -77.88851},
                       expected_y1 = {35.54080, 34.02629, 34.45901, 34.24265, 34.24265, 34.24265},
                       expected_x2 = {-79.12938, -77.96151, -77.88851, -77.97124, -77.96637, -77.96637},
                       expected_y2 = {35.54080, 34.02629, 34.51331, 34.23416, 34.28168, 34.28168};

        // Computed values
        tuple<vector<double>, vector<double>>
        test_vals1 = spress::decode(8, h1, bb[0], bb[1], bb[2], bb[3]),
        test_vals2 = spress::decode(256, h2, bb[0], bb[1], bb[2], bb[3]);
        
        for (size_t i = 0; i < h1.size(); ++i) {
            expect_true(std::get<0>(test_vals1)[i] == Approx(expected_x1[i]).epsilon(eps));
            expect_true(std::get<1>(test_vals1)[i] == Approx(expected_y1[i]).epsilon(eps));
            expect_true(std::get<0>(test_vals2)[i] == Approx(expected_x2[i]).epsilon(eps));
            expect_true(std::get<1>(test_vals2)[i] == Approx(expected_y2[i]).epsilon(eps));
        }
    }
}


context("libspress/compress")
{
    test_that("compression works as expected")
    {
        vector<size_t> test_vals = {8, 32, 317, 2, 654, 12, 9};
        tuple<vector<size_t>, vector<size_t>> compressed = spress::compress(test_vals);

        vector<size_t> expected_indices = {2, 6, 1, 3, 20, 285, 337},
                       expected_order = {3, 0, 6, 5, 1, 2, 4};
        for (size_t i = 0; i < test_vals.size(); ++i) {
            expect_true(std::get<0>(compressed)[i] == expected_indices[i]);
            expect_true(std::get<1>(compressed)[i] == expected_order[i]);
        }
    }

    test_that("decompression works as expected")
    {
        vector<size_t> test_indices = {2, 6, 1, 3, 20, 285, 337},
                       test_order = {3, 0, 6, 5, 1, 2, 4},
                       expected = {8, 32, 317, 2, 654, 12, 9},
                       decompressed = spress::decompress(test_indices, test_order);

        for (size_t i = 0; i < expected.size(); ++i) {
            expect_true(decompressed[i] == expected[i]);
        }
    }
}