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