#' @param coords Column indices or names corresponding to coordinates
#' @param attach If `TRUE`, attaches index to data frame as new column
#' @rdname encode
#' @export
encode.data.frame <- function(x, ..., n = 4L, coords = c(1, 2), attach = FALSE) {
    h <- encode(x[[coords[1]]], x[[coords[2]]], ..., n = n)
    if (attach) {
        x$h <- h
        attr(x, "spress_extent") <- c(
            "xmax" = max(x[[coords[1]]]), "xmin" = min(x[[coords[1]]]),
            "ymax" = max(x[[coords[2]]]), "ymin" = min(x[[coords[2]]])
        )
        attr(x, "class") <- c("spress_tbl", "data.frame")
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

    attr(en, "spress_n") <- n
    attr(en, "spress_extent") <- c(
        "xmax" = max(x), "xmin" = min(x),
        "ymax" = max(y), "ymin" = min(y)
    )
    attr(en, "class") <- c("spress_index", "integer")

    en
}