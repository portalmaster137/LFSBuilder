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
    std::string lfs_package_tar = lfs_sources + "/lfs-packages.tar";
    run_cmd_with_prefix("mkdir -v " + lfs_sources, "[Sources]");
    run_cmd_with_prefix("chmod -v a+wt " + lfs_sources, "[Sources]");
    run_cmd_with_prefix("curl " + lfsurl + " -o " + lfs_package_tar, "[Sources]");
    run_cmd_with_prefix("tar -xpvf " + lfs_package_tar);

    return true;
}
