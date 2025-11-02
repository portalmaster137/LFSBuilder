//
// Created by Junie (AI) on 11/2/25.
//
#pragma once

#include <stdbool.h>

// Runs a comprehensive host tools check by executing an embedded Bash script
// that verifies versions and presence of critical development tools.
//
// Returns true on success (all checks pass), false on failure (any check fails).
// The script's output is printed to the parent process's stdout/stderr.
bool run_host_tools_check();
