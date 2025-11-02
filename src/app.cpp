//
// Created by porta on 11/2/25.
//

#include "Consts.h"
#include "ToolsCheck.h"
#include "argparse.hpp"
#include "Partitioning.h"

int main(int argc, char *argv[]) {
    argparse::ArgumentParser app("LFSBuilder");
    app.add_argument("block_device");
    try {
        app.parse_args(argc, argv);
    } catch (std::exception &e) {
        FAIL_FATAL("Invalid argument: %s", e.what());
    }
    auto block_device = app.get<std::string>("block_device");
    std::cout << "Block device: " << block_device << std::endl;


    //Step 1: Run host tools check
    if (bool res = run_host_tools_check(); !res) {
        FAIL_FATAL("Failed to run tools check");
    }

    if (bool res = setup_partitions(block_device); !res) {
        FAIL_FATAL("Failed to setup partitions");
    }

    // Step 2: Mount partitions to LFS and LFS/boot
    if (bool res = mount_partitions(); !res) {
        FAIL_FATAL("Failed to mount partitions");
    }
}
