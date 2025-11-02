//
// Created by porta on 11/2/25.
//
// Common constants and helper macros

#ifndef LFSBUILDER_CONSTS_H
#define LFSBUILDER_CONSTS_H

#include <cstdio>
#include <cstdlib>

// Default LFS mount point
#ifndef LFS
#define LFS "/mnt/lfs"
#endif

// FAIL_FATAL: Print a fatal error message to stderr and terminate the process
// Usage examples:
//   FAIL_FATAL("Unexpected null pointer");
//   FAIL_FATAL("Failed to open %s: %s", path.c_str(), strerror(errno));
// Notes:
// - Always appends a trailing newline.
// - Exits with EXIT_FAILURE.
#ifndef FAIL_FATAL
#define FAIL_FATAL(fmt, ...)                                                     \
    do {                                                                         \
        std::fprintf(stderr, "FATAL: " fmt "\n", ##__VA_ARGS__);                 \
        std::fflush(stderr);                                                     \
        std::exit(EXIT_FAILURE);                                                 \
    } while (0)
#endif



#endif //LFSBUILDER_CONSTS_H