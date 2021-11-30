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