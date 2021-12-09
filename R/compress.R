compress <- function(x, ...) {
    UseMethod("compress", x)
}

compress.integer <- function(x, ...) {
    .Call(`_spress_compress_`, x)
}