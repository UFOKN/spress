#nolint start

#' Pull geometry out of `sf` object
#' @param sf_obj `sf`/`sfc`/`sfg` object
#' @return Geometry object
#' @keywords internal
.pull_geometry <- function(sf_obj) UseMethod(".pull_geometry", sf_obj)

#' @export
.pull_geometry.sf <- function(sf_obj) {
    sf_col <- attr(sf_obj, "sf_column", exact = TRUE)
    sf_obj <- sf_obj[[sf_col]]
    .Class <- class(sf_obj)
    NextMethod()
}

#' @export
.pull_geometry.sfc <- function(sf_obj) sf_obj

#' @export
.pull_geometry.sfg <- function(sf_obj) sf_obj

#' Pull geometry type out of `sf` object.
#' @param sf_obj `sf`/`sfc`/`sfg` object
#' @return Geometry type string
#' @keywords internal
.pull_geotype <- function(sf_obj) UseMethod(".pull_geotype", sf_obj)

#' @export
.pull_geotype.sf <- function(sf_obj) {
    sf_obj <- .pull_geometry(sf_obj)
    .Class <- class(sf_obj)
    NextMethod()
}

#' @export
.pull_geotype.sfc <- function(sf_obj) {
    x <- class(sf_obj)
    gsub("^sfc_", "", x[x != "sfc"])
}

#' @export
.pull_geotype.sfg <- function(sf_obj) {
    x <- class(sf_obj)
    x[x != "sfg" & !grepl("^X", x)]
}

#==============================================================================

#' Pull CRS out of `sf` object.
#' @param sf_obj `sf`/`sfc`/`sfg` object
#' @return CRS object
#' @keywords internal
.pull_crs <- function(sf_obj) UseMethod(".pull_crs", sf_obj)

#' @export
.pull_crs.sf <- function(sf_obj) {
    sf_obj <- .pull_geometry(sf_obj)
    .Class <- class(sf_obj)
    NextMethod()
}

#' @export
.pull_crs.sfc <- function(sf_obj) {
    attr(sf_obj, "crs")
}

#' @export
.pull_crs.sfg <- function(sf_obj) NA

#==============================================================================

.pull_coords <- function(sf_obj) UseMethod(".pull_coords", sf_obj)

#' @export
.pull_coords.sf <- function(sf_obj) {
    sf_obj <- .pull_geometry(sf_obj)
    .Class <- class(sf_obj)
    NextMethod()
}

#' @export
.pull_coords.sfc <- function(sf_obj) {
    do.call(rbind, lapply(seq_len(length(sf_obj)), function(i) {
        cbind(sfc_id = i, .pull_coords(sf_obj[[i]]))
    }))
}

#' @export
.pull_coords.sfg <- function(sf_obj) {
    sf_type <- .pull_geotype(sf_obj)
    sf_ls   <- data.frame()

    if (sf_type == "POINT") {
        # vector
        sf_ls <- data.frame(
            sfg_id = 1,
            x      = sf_obj[1],
            y      = sf_obj[2]
        )
    } else if (sf_type %in% c("MULTIPOINT", "LINESTRING")) {
        # matrix
        sf_ls <- data.frame(
            sfg_id = 1,
            x      = sf_obj[, 1],
            y      = sf_obj[, 2]
        )
    } else if (sf_type == "POLYGON") {
        # list containing single matrix
        sf_ls <- data.frame(
            sfg_id = 1,
            x      = sf_obj[[1]][, 1],
            y      = sf_obj[[1]][, 2]
        )
    } else if (sf_type %in% c("MULTILINESTRING", "MULTIPOLYGON")) {
        # list of matrices
        sf_len <- length(sf_obj)
        sf_ls  <- do.call(rbind, lapply(seq_len(sf_len), function(i) {
            obj_len <- length(sf_obj[[i]])
            do.call(rbind, lapply(seq_len(obj_len), function(j) {
                data.frame(
                    sfg_id = i,
                    x      = sf_obj[[i]][[j]][, 1],
                    y      = sf_obj[[i]][[j]][, 2]
                )
            }))
        }))
    } else {
        stop(sprintf("Geometry type %s not supported", sf_type), call. = FALSE)
    }

    sf_ls
}

#nolint end