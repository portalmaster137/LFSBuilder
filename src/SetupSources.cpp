//
// Created by porta on 11/2/25.
//
#include "SetupSources.h"

#include <string>

#include "Consts.h"
#include "RunCmd.h"


bool setup_sources() {
    std::string lfs = std::string(LFS);
    std::string lfsurl = std::string(LFSPACKAGELINK);
    std::string lfs_sources = lfs + "/sources";
    run_cmd_with_prefix("mkdir -v " + lfs_sources, "[Sources]");
    run_cmd_with_prefix("chmod -v a+wt " + lfs_sources, "[Sources]");
    run_cmd_with_prefix("curl " + lfs_sources + " -o " + lfs_sources + "/lfs-packages.tar", "[Sources]");

    return true;
}
