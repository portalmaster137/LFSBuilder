//
// Created by Junie (AI) on 11/2/25.
//
#pragma once

#include <string>

// Executes the given shell command using std::system().
// - Prints the command before executing it.
// - Returns true if the command exits with status 0, false otherwise.
//
// The optional prefix is printed before the command, e.g., "[partition] $ <cmd>".
// When prefix is nullptr or empty, it prints just "$ <cmd>".
bool run_cmd_with_prefix(const std::string &cmd, const char* prefix = nullptr);

// Convenience wrapper without a prefix.
bool run_cmd(const std::string &cmd);
