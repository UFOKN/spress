
<!-- README.md is generated from README.Rmd. Please edit that file -->

# spress

<!-- badges: start -->

[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
[![CRAN
status](https://www.r-pkg.org/badges/version/spress)](https://CRAN.R-project.org/package=spress)
<!-- badges: end -->

**spress** provides utilities for encoding and compressing geospatial
objects, such as `sf` objects.

## Installation

This package requires **C++11** for compilation. Once you have that
installed, you can install the development version from
[GitHub](https://github.com/) with `devtools` or `pak`:

``` r
# pak
pak::pkg_install("program--/spress")

# devtools
devtools::install_github("program--/spress")
```

## C++ Library

Included in this package is the header-only C++ library `libspate`,
which contains the foundational code used for encoding/decoding, and was
developed specifically for this package.

This can be found at `inst/include/spate.hpp` from the root of this
repository. Unit testing for this library is done with
[Catch2](https://github.com/catchorg/Catch2) and are contained in
`inst/cpp-tests`.

> Note that the C++ code for this package contains purely the algorithm
> for encoding and decoding. The current roadmap for this package
> includes making the C++ library fully vendorable.

## Attribution

This package was inspired by the paper:

Liu, Dongge, Tao Wang, Xiaojuan Li, Yeqing Ni, Yanping Li, and Zhao Jin.
2020. “A Multiresolution Vector Data Compression Algorithm Based on
Space Division” ISPRS International Journal of Geo-Information 9, no.
12: 721. <https://doi.org/10.3390/ijgi9120721>
