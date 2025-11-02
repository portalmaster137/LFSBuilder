//
// Created by porta on 11/2/25.
//
#include "BuildSources.h"
#include "Consts.h"
#include "RunCmd.h"

bool build_binutils_pass_1() {
    std::string lfs = std::string(LFS);
    run_command("tar -xpvf binutils-2.45.tar.xz", lfs + "/sources", "[Binutils]");

    return true;
}
