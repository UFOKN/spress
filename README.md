
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
pak::pkg_install("UFOKN/spress")

# devtools
devtools::install_github("UFOKN/spress")
```

## Usage

**spress** offers *both* encoding and compressing utilities. For
clarity, in the context of **spress**, *encoding* refers to projecting
2-dimensional coordinates to a 1-dimensional system (particularly,
geographic coordinates to a Hilbert Curve index); whereas, *compressing*
refers to the process for reducing the object and file size of the
encoded object.

### Encoding

At a high level, **spress** performs the following algorithm for
encoding:

1.  Given a set of points *P*
2.  Retrieve *P*’s extent/bounding box
3.  Create a *2^n* x *2^n* grid with centroids aligned to the extent’s
    corner points
4.  Index the grid to a Hilbert Curve
5.  Assign each point in *P* to the closest index on the Hilbert Curve

A visualization of this algorithm is shown below:

![](man/figures/process.png) After encoding, the data necessary to be
stored is reduced down to:

1.  The set of points *P*’s **extent**;
2.  The ***n*** value used;
3.  and, the list of **indices**.

With this set of data, the decoding process is exactly the reverse of
the encoding algorithm.

In the visual example above, an *n* of **4** was used (since *2^4 = 16*,
and we have a 16x16 grid), however, note that the maximum possible error
*E* converges toward 0 as the *extent* becomes smaller, and as *n*
grows. Thus, we can achieve a much more accurate encoding scheme by
modifying these when possible.

> Realistically, this algorithm is computationally expensive if the
> **extent** and ***n*** are large. However, this is mitigated via
> bitwise operations and taking advantage of efficient C++ structures.
> For the specifics of this, see `libspate`’s source code in
> `inst/include/spate.hpp`.

#### Encoding Example

``` r
# Random set of points
P <- data.frame(
  X = c(-77.8015434499876,
        -77.8707250008617,
        -77.9212227055911,
        -77.9385055000025,
        -77.8452515318622,
        -77.8715886279598),
  Y = c(34.2961615000021,
        34.1818796104794,
        34.1447587911948,
        34.2165775000314,
        34.2218266693184,
        34.2632856737062)
)

# n = 8 --> 2^8 x 2^8 grid --> 256x256 grid
n <- 8L

# Encode using vectors
spress::encode(P$X, P$Y, n = n)
#> > spress encoded coordinates: 6 indices
#> # Grid Dimensions: 256 x 256 (65536)
#> # Extent:          -77.80, -77.94, 34.30, 34.14
#>       Index  Coordinates
#> [1]:  43690  (-77.92, 34.14)
#> [2]:   6827  (-77.87, 34.18)
#> [3]:   3754  (-77.94, 34.22)
#> [4]:  16323  (-77.85, 34.22)
#> [5]:  34040  (-77.80, 34.30)
#> # … with 1 more index

# We can also use the data.frame directly and attach to it
encoded_P <- spress::encode(P, coords = c("X", "Y"), attach = FALSE)

encoded_df <- spress::encode(P, coords = c("X", "Y"), attach = TRUE)
encoded_df
#>           X        Y   h
#> 1 -77.80154 34.29616 170
#> 2 -77.87073 34.18188  37
#> 3 -77.92122 34.14476  14
#> 4 -77.93851 34.21658  63
#> 5 -77.84525 34.22183 132
#> 6 -77.87159 34.26329 111
```

#### Decoding Example

We will use `encoded_P` and `encoded_df` from the encoding example If we
decode an object that used `spress::encode` to encode, there’s no need
to pass `extent` or `n` to the decoding function. These values are
stored in the attributes `spress_extent` and `spress_n`, respectively.

``` r
spress::decode(encoded_P)
#>           x        y
#> 1 -77.80154 34.29616
#> 2 -77.87459 34.18513
#> 3 -77.92024 34.14476
#> 4 -77.93851 34.21541
#> 5 -77.84720 34.22551
#> 6 -77.87459 34.26588

# Columns `x` and `y` are the new decoded coordinate columns
# The argument `index` refers to the column index of the encoded index values
spress::decode(encoded_df, index = 3)[, c("x", "y")]
#>           x        y
#> 1 -77.80154 34.29616
#> 2 -77.87459 34.18513
#> 3 -77.92024 34.14476
#> 4 -77.93851 34.21541
#> 5 -77.84720 34.22551
#> 6 -77.87459 34.26588
```

### Compression

> Compression is **not** implement yet as of 11/30/21.

For compression, **spress** performs the following algorithm:

1.  Given a set of points *P*
2.  Encode *P* based on the encoding algorithm
3.  Create an ordering index for order preservation upon decompression
4.  Sort indices by the encoding index, such that each point in *P*
    follows along corresponding Hilbert Curve
5.  Keep the first encoding index and get the difference between every
    successive point

This algorithm returns a set of indices that (generally) should be small
integers. The returned table can then be written to any file format,
preferably a columnar format (such as [Apache
Parquet](https://parquet.apache.org/)), and additionally compressed
using another compression library (such as [xz](https://tukaani.org/xz/)
or [snappy](https://github.com/google/snappy)).

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
