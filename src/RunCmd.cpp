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

namespace {
    // Escape single quotes for safe inclusion inside a single-quoted shell string.
    // In POSIX shell, to insert a single quote inside single quotes, close them, insert '\'' and reopen.
    std::string shell_single_quote(const std::string &s) {
        std::string out;
        out.reserve(s.size() + 8);
        for (char c : s) {
            if (c == '\'') {
                out += "'\\''"; // end quote, escaped quote, reopen
            } else {
                out += c;
            }
        }
        return out;
    }
}

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

bool run_command_as(const std::string &cmd, const std::string &workdir, const std::string &user, const char* prefix) {
    namespace fs = std::filesystem;
    fs::path previous;
    try {
        previous = fs::current_path();
        fs::current_path(workdir);
    } catch (const std::exception &e) {
        std::fprintf(stderr, "[run_command_as] failed to change directory to '%s': %s\n", workdir.c_str(), e.what());
        return false;
    }

    // Construct effective prefix like run_command
    std::string effective_prefix;
    if (prefix && *prefix) {
        effective_prefix = prefix;
    } else {
        effective_prefix = "[" + workdir + "]";
    }

    // Print the intended command as-is (without the runuser wrapper)
    if (!effective_prefix.empty()) {
        std::fprintf(stderr, "%s $ %s\n", effective_prefix.c_str(), cmd.c_str());
    } else {
        std::fprintf(stderr, "$ %s\n", cmd.c_str());
    }

#ifdef __unix__
    std::string wrapped = "runuser -u " + user + " -- sh -lc '" + shell_single_quote(cmd) + "'";
    int rc = std::system(wrapped.c_str());
    bool ok;
    if (rc == -1) {
        ok = false;
    } else if (WIFEXITED(rc)) {
        ok = (WEXITSTATUS(rc) == 0);
    } else {
        ok = false;
    }
#else
    std::fprintf(stderr, "[run_command_as] warning: platform not supported; running as current user instead.\n");
    int rc = std::system(cmd.c_str());
    bool ok = (rc == 0);
#endif

    // Restore previous working directory
    try {
        fs::current_path(previous);
    } catch (const std::exception &e) {
        std::fprintf(stderr, "[run_command_as] warning: failed to restore previous directory '%s': %s\n", previous.string().c_str(), e.what());
    }

    return ok;
}
