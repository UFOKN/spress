decompress <- function(x, ...) {
    UseMethod("decompress", x)
}

decompress.integer <- function(x, order, ...) {
  .Call(`_spress_decompress_`, x, order)
}