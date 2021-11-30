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

#' @rdname encode
#' @export
encode.default <- function(x, ...) {
    stop()
}

#' @param coords Column indices or names corresponding to coordinates
#' @param attach If `TRUE`, attaches index to data frame as new column
#' @rdname encode
#' @export
encode.data.frame <- function(x, ..., n = 4L, coords = c(1, 2), attach = FALSE) {
    h <- encode(x[[coords[1]]], x[[coords[2]]], ..., n = n)
    if (attach) {
        x$h <- h
        x
    } else {
        h
    }
}

#' @param y A `numeric` vector
#' @rdname encode
#' @export
encode.numeric <- function(x, y, ..., n = 4L) {
    en <- .Call(`_spress_encode_`, 2L ^ n, x, y)

    attr(en, "spress_n") <- 2L ^ n
    attr(en, "spress_extent") <- c(
        "xmax" = max(x), "xmin" = min(x),
        "ymax" = max(y), "ymin" = min(y)
    )

    en
}