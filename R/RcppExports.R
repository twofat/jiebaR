# This file was generated by Rcpp::compileAttributes
# Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#' @title Files encoding detection
#' @description The function detect the encoding of input files.
#' @param file A file path.
#' @return The encoding of file
#' @author Wu Yongwei, Qin wenfeng
#' @references \url{https://github.com/adah1972/tellenc}
#' @export
filecoding <- function(file) {
    .Call('jiebaR_filecoding', PACKAGE = 'jiebaR', file)
}

mp_ptr <- function(dict, user) {
    .Call('jiebaR_mp_ptr', PACKAGE = 'jiebaR', dict, user)
}

mp_cut <- function(x, cutter) {
    .Call('jiebaR_mp_cut', PACKAGE = 'jiebaR', x, cutter)
}

mix_ptr <- function(dict, model, user) {
    .Call('jiebaR_mix_ptr', PACKAGE = 'jiebaR', dict, model, user)
}

mix_cut <- function(x, cutter) {
    .Call('jiebaR_mix_cut', PACKAGE = 'jiebaR', x, cutter)
}

query_ptr <- function(dict, model, n) {
    .Call('jiebaR_query_ptr', PACKAGE = 'jiebaR', dict, model, n)
}

query_cut <- function(x, cutter) {
    .Call('jiebaR_query_cut', PACKAGE = 'jiebaR', x, cutter)
}

hmm_ptr <- function(model) {
    .Call('jiebaR_hmm_ptr', PACKAGE = 'jiebaR', model)
}

hmm_cut <- function(x, cutter) {
    .Call('jiebaR_hmm_cut', PACKAGE = 'jiebaR', x, cutter)
}

tag_ptr <- function(dict, model, user) {
    .Call('jiebaR_tag_ptr', PACKAGE = 'jiebaR', dict, model, user)
}

tag_tag <- function(x, cutter) {
    .Call('jiebaR_tag_tag', PACKAGE = 'jiebaR', x, cutter)
}

tag_file <- function(x, cutter) {
    .Call('jiebaR_tag_file', PACKAGE = 'jiebaR', x, cutter)
}

key_ptr <- function(n, dict, model, idf, stop) {
    .Call('jiebaR_key_ptr', PACKAGE = 'jiebaR', n, dict, model, idf, stop)
}

key_tag <- function(x, cutter) {
    .Call('jiebaR_key_tag', PACKAGE = 'jiebaR', x, cutter)
}

key_cut <- function(x, cutter) {
    .Call('jiebaR_key_cut', PACKAGE = 'jiebaR', x, cutter)
}

sim_ptr <- function(dict, model, idf, stop) {
    .Call('jiebaR_sim_ptr', PACKAGE = 'jiebaR', dict, model, idf, stop)
}

sim_sim <- function(code, topn, cutter) {
    .Call('jiebaR_sim_sim', PACKAGE = 'jiebaR', code, topn, cutter)
}

sim_distance <- function(lhs, rhs, topn, cutter) {
    .Call('jiebaR_sim_distance', PACKAGE = 'jiebaR', lhs, rhs, topn, cutter)
}

