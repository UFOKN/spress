#include <spate.hpp>
#include <cpp11.hpp>
using namespace cpp11;

[[cpp11::register]]
integers encode_(size_t n, doubles x, doubles y)
{
    return as_sexp(spate::encode(n, as_cpp<vector<double>>(x), as_cpp<vector<double>>(y)));
}

[[cpp11::register]]
data_frame decode_(size_t n, integers h, doubles bb)
{
    using namespace cpp11::literals;

    vector<size_t> h_vec(h.begin(), h.end());
    tuple<vector<double>, vector<double>> decoded = spate::decode(
        n,
        h_vec,
        bb["xmax"],
        bb["xmin"],
        bb["ymax"],
        bb["ymin"]
    );

    writable::data_frame coords({
        "x"_nm = std::get<0>(decoded),
        "y"_nm = std::get<1>(decoded)
    });

    return coords;
}

[[cpp11::register]]
data_frame grid_(size_t n, doubles bb)
{
    using namespace cpp11::literals;
    tuple<vector<double>, vector<double>, vector<size_t>, vector<size_t>, vector<size_t>> hgrid =
        spate::grid::grid(
            n,
            bb["xmax"], bb["xmin"],
            bb["ymax"], bb["ymin"]
        );

    writable::data_frame grd({
        "x"_nm  = std::get<0>(hgrid),
        "y"_nm  = std::get<1>(hgrid),
        "gx"_nm = std::get<2>(hgrid),
        "gy"_nm = std::get<3>(hgrid),
        "h"_nm  = std::get<4>(hgrid)
    });

    grd.attr("spress_n") = n;

    return grd;
}