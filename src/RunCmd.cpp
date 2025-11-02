//
// Created by Junie (AI) on 11/2/25.
//
#include "RunCmd.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
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

bool run_command(const std::string &cmd, const std::string &workdir, const char* prefix) {
    namespace fs = std::filesystem;
    fs::path previous;
    try {
        previous = fs::current_path();
        fs::current_path(workdir);
    } catch (const std::exception &e) {
        std::fprintf(stderr, "[run_command] failed to change directory to '%s': %s\n", workdir.c_str(), e.what());
        return false;
    }

    // Construct effective prefix: use provided one if any; otherwise, show the workdir.
    std::string effective_prefix;
    if (prefix && *prefix) {
        effective_prefix = prefix;
    } else {
        effective_prefix = "[" + workdir + "]";
    }

    bool ok = run_cmd_with_prefix(cmd, effective_prefix.c_str());

    // Restore previous working directory, report but do not override command result.
    try {
        fs::current_path(previous);
    } catch (const std::exception &e) {
        std::fprintf(stderr, "[run_command] warning: failed to restore previous directory '%s': %s\n", previous.string().c_str(), e.what());
    }

    return ok;
}
