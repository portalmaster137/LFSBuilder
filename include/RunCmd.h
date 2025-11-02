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

// Executes the given command with the working directory set to `workdir`.
// - Prints the command, prefixed by "[<workdir>] $" unless a custom `prefix` is provided.
// - Returns true if the command exits with status 0, false otherwise.
// - This uses the platform shell to change directory and run the command in a single invocation.
bool run_command(const std::string &cmd, const std::string &workdir, const char* prefix = nullptr);

// Executes the given command as the specified user within `workdir`.
// - Assumes the program runs as root and uses `runuser` (or equivalent) under the hood.
// - Prints the command with the same prefix rules as `run_command` unless a custom `prefix` is given.
// - Returns true if the command exits with status 0, false otherwise.
bool run_command_as(const std::string &cmd, const std::string &workdir, const std::string &user, const char* prefix = nullptr);
