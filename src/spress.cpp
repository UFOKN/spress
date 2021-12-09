#include <spate.hpp>
#include <cpp11.hpp>
using namespace cpp11;

[[cpp11::register]]
data_frame compress_(integers encoded)
{
    using namespace cpp11::literals;
    vector<size_t> stdv_e(encoded.begin(), encoded.end());
    tuple<vector<size_t>, vector<size_t>> compressed = spate::compress(stdv_e);
    
    return writable::data_frame({
        "index"_nm = std::get<0>(compressed),
        "order"_nm = std::get<1>(compressed)
    });
}

[[cpp11::register]]
integers decompress_(integers x, integers order)
{
    vector<size_t> stdv_x(x.begin(), x.end());
    vector<size_t> stdv_o(order.begin(), order.end());

    return as_sexp(spate::decompress(stdv_x, stdv_o));
}

[[cpp11::register]]
integers encode_(size_t n, doubles x, doubles y)
{
    return as_sexp(
        spate::encode(
            n,
            as_cpp<vector<double>>(x),
            as_cpp<vector<double>>(y)
        )
    );
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

    return writable::data_frame({
        "x"_nm = std::get<0>(decoded),
        "y"_nm = std::get<1>(decoded)
    });
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