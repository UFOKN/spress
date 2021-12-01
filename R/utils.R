#' @keywords internal
.validate_extent <- function(h, extent = NULL) {
    attr_extent <- attr(h, "spress_extent", exact = TRUE)
    if (is.null(attr_extent) & is.null(extent)) {
        stop("`extent` is missing.")
    } else if (!is.null(attr_extent) & is.null(extent)) {
        extent <- attr_extent
    }

    extent_nms <- names(extent)
    expect_nms <- c("xmax", "xmin", "ymax", "ymin")
    if (!all(expect_nms %in% extent_nms)) {
        stop(paste0(
            "`extent` must have ",
            "`xmax`, `xmin`, `ymax`, and `ymin`",
            " as names."
        ))
    }
    rm(extent_nms, expect_nms)

    extent
}

#' @keywords internal
.validate_n <- function(h, n) {
    attr_n <- attr(h, "spress_n", exact = TRUE)
    if (!is.null(attr_n)) {
        n <- attr_n
    }
    rm(attr_n)

    2L ^ as.integer(n)
}

#' @keywords internal
.to_json <- function(x) {
    n   <- attr(x, "spress_n", exact = TRUE)
    ext <- attr(x, "spress_extent", exact = TRUE)
    geo <- attr(x, "spress_geometry", exact = TRUE)

    sprintf(
        "{\"n\": %s, \"extent\": [%s], \"geometry\": \"%s\", \"index\": [%s]}",
        n,
        paste0(ext, collapse = ", "),
        if (!is.null(geo)) geo else NA_character_,
        paste0(x, collapse = ", ")
    )
}