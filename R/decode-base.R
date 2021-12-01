#' @rdname decode
#' @export
decode.spress_index <- function(x, ...) {
    extent <- attr(x, "spress_extent", exact = TRUE)
    n      <- attr(x, "spress_n", exact = TRUE)
    decode(as.integer(x), extent = extent, n = n)
}

#' @param index Index or name of column containing the Hilbert curve index
#' @rdname decode
#' @export
decode.data.frame <- function(x, ..., extent, index = length(x), n = 4L) {
    encoded <- x[[index]]

    if (is.null(encoded)) {
        stop("Column defined by `index` does not exist.")
    }

    if (typeof(encoded) != "integer") {
        stop("Column defined by `index` is not of type `integer`.")
    }

    # Dispatch to atomic methods
    decoded <- decode(encoded, ..., extent = extent, n = n)
    x$x     <- decoded$x
    x$y     <- decoded$y
    x
}

#' @rdname decode
#' @export
decode.double <- function(x, ..., extent, n = 4L) {
    .Class <- "integer"
    x      <- as.integer(x)
    NextMethod()
}

#' @rdname decode
#' @export
decode.integer <- function(x, ..., extent, n = 4L) {
    extent <- .validate_extent(x, if (!missing(extent)) extent)
    n      <- .validate_n(x, n)

    .Call(`_spress_decode_`, n, x, extent)
}