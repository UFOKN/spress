#' @importFrom utils head
#' @export
print.spress_index <- function(x, ...) {
    x_n    <- 2L ^ attr(x, "spress_n", exact = TRUE)
    x_ext  <- attr(x, "spress_extent", exact = TRUE)
    x_geom <- attr(x, "spress_geometry", exact = TRUE)
    head_pts    <- head(x, n = 5)
    decoded_pts <- grid_(x_n, x_ext)[
        grid_(x_n, x_ext)$h %in% head_pts, c("x", "y")
    ]

    cat("> spress encoded coordinates:", length(x), if (length(x) == 0 | length(x) > 1) "indices" else "index")
    cat("\n")
    cat("# Grid Dimensions:", x_n, "x", x_n, paste0("(", x_n ^ 2, ")"))
    cat("\n")
    cat("# Extent:         ", paste(formatC(x_ext, digits = 2, format = "f"), collapse = ", "))
    cat("\n")
    if (!is.null(x_geom)) {
        cat("# Geometry:       ", toupper(x_geom))
        cat("\n")
    }
    cat(sprintf(paste0("%", max(nchar(x)) + 6, "s"), "Index"))
    cat("  Coordinates\n")
    cat(paste0(
        "[", seq_len(length(head_pts)), "]:",
        sprintf(paste0("%", max(nchar(x)) + 2, "s"), head_pts),
        "  (",
        formatC(decoded_pts$x, digits = 2, format = "f"), ", ",
        formatC(decoded_pts$y, digits = 2, format = "f"), ")",
        "\n"
    ), sep = "")
    if (length(x) > 5) {
        cat("# \u2026 with",
            length(x) - 5,
            "more",
            if (length(x) - 5 == 0 | length(x) - 5 > 1) "indices" else "index")
    }

    invisible(x)
}