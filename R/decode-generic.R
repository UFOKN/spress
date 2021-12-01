#' Decode a Hilbert curve index to coordinates
#' @param x A `data.frame` or `numeric` vector
#' @param ... Unused
#' @param extent Extent of the original dataset.
#'               If encoded with `encode`, then
#'               the attribute `spress_extent` is used.
#' @param n Level of space division, must be between 0 and 16.
#'          If encoded with `encode`, then
#'          the attribute `spress_n` is used.
#' @export
decode <- function(x, ..., extent, n = 4L) {
    if (n > 16 | n < 0) {
        stop("`n` is invalid. Must be between 0 and 16.")
    }

    UseMethod("decode", x)
}