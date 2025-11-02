//
// Created by Junie (AI) on 11/2/25.
//
#include "RunCmd.h"

#include <cstdio>
#include <cstdlib>
#ifdef __unix__
#include <sys/wait.h>
#endif

bool run_cmd_with_prefix(const std::string &cmd, const char* prefix) {
    if (prefix && *prefix) {
        std::fprintf(stderr, "%s $ %s\n", prefix, cmd.c_str());
    } else {
        std::fprintf(stderr, "$ %s\n", cmd.c_str());
    }
    int rc = std::system(cmd.c_str());
#ifdef __unix__
    if (rc == -1) return false;
    if (WIFEXITED(rc)) return WEXITSTATUS(rc) == 0;
    return false;
#else
    return rc == 0;
#endif
}

bool run_cmd(const std::string &cmd) {
    return run_cmd_with_prefix(cmd, nullptr);
}
