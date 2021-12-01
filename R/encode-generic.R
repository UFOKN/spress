#' Encode coordinates to a Hilbert curve index
#' @param x A `data.frame` or `numeric` vector
#' @param ... Unused
#' @param n Level of space division, must be between 0 and 16.
#' @export
encode <- function(x, ..., n = 4L) {
    if (n > 16 | n < 0) {
        stop("`n` is invalid. Must be between 0 and 16.")
    }

    UseMethod("encode", x)
}