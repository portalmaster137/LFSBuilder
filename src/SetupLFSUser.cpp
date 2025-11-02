//
// Created by porta on 11/2/25.
//

#include "SetupLFSUser.h"

#include "Consts.h"
#include "RunCmd.h"

bool setup_lfs_user() {
    std::string lfs = std::string(LFS);
    run_cmd_with_prefix("groupadd lfs", "[User]");
    run_cmd_with_prefix("useradd -s /bin/bash -g lfs -m -k /dev/null lfs", "[User]");
    run_cmd_with_prefix("chown -v lfs " + lfs + "/{usr{,/*},var,etc,tools}", "[User]");
    run_cmd_with_prefix("case $(uname -m) in x86_64) chown -v lfs " + lfs + "/lib64 ;; esac", "[User]");
    return true;
}
