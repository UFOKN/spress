#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/spate.hpp"

TEST_CASE("grid/sequence generation", "Test that the sequence from 1 to 10 with length 10 is correct")
{
    vector<double> test_seq = spate::grid::seq(1.0, 10.0, 10);
    vector<double> expected = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

    for (int i = 0; i < 10; ++i) {
        REQUIRE( test_seq[i] == expected[i] );
    }
}

TEST_CASE("grid/indexing", "Ensure indexing is correct")
{
    REQUIRE( spate::hilbert::pos_index(4, 1, 2) == 7 );
    REQUIRE( spate::hilbert::pos_index(8, 3, 4) == 31 );
}

TEST_CASE("grid/positioning", "Ensure positioning is correct")
{
    size_t test_x[2];
    size_t test_y[2];

    spate::hilbert::index_pos(4, 7, &test_x[0], &test_y[0]);
    spate::hilbert::index_pos(8, 31, &test_x[1], &test_y[1]);

    REQUIRE(test_x[0] == 1);
    REQUIRE(test_y[0] == 2);
    REQUIRE(test_x[1] == 3);
    REQUIRE(test_y[1] == 4);
}

TEST_CASE("grid/closest", "Ensure closest function works as expected")
{
    vector<double> test_vals = {1.0, 5.0, 6.3, 7.2, 10.0};
    REQUIRE(spate::closest(test_vals, 6) == 2);
    REQUIRE(spate::closest(test_vals, 5.6) == 1);
    REQUIRE(spate::closest(test_vals, 7.1) == 3);
    REQUIRE(spate::closest(test_vals, 8) == 3);
    REQUIRE(spate::closest(test_vals, 9) == 4); // fails without cast
    REQUIRE(spate::closest(test_vals, 9.0) == 4);
    REQUIRE(spate::closest(test_vals, 9.7) == 4);   
}

TEST_CASE("compression", "Ensure compression works as expected")
{
    vector<size_t> test_vals = {8, 32, 317, 2, 654, 12, 9};
    tuple<vector<size_t>, vector<size_t>> compressed = spate::compress(test_vals);

    vector<size_t> expected_indices = {2, 6, 1, 3, 20, 285, 337};
    vector<size_t> expected_order = {3, 0, 6, 5, 1, 2, 4};

    for (size_t i = 0; i < test_vals.size(); ++i) {
        REQUIRE(std::get<0>(compressed)[i] == expected_indices[i]);
        REQUIRE(std::get<1>(compressed)[i] == expected_order[i]);
    }
}

TEST_CASE("decompression", "Ensure decompression works as expected")
{
    vector<size_t> test_indices = {2, 6, 1, 3, 20, 285, 337};
    vector<size_t> test_order = {3, 0, 6, 5, 1, 2, 4};
    vector<size_t> expected = {8, 32, 317, 2, 654, 12, 9};
    vector<size_t> decompressed = spate::decompress(test_indices, test_order);

    for (size_t i = 0; i < expected.size(); ++i) {
        REQUIRE(decompressed[i] == expected[i]);
    }
}