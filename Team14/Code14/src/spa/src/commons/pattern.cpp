//
// Created by gabri on 11/9/2022.
//

#include "pattern.h"

#include <utility>

Pattern::Pattern(std::string var_name, int stmt_no, std::string expr)
    : var_name_(std::move(var_name)), stmt_no_(stmt_no), expr_(std::move(expr)) {}
