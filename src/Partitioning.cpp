//
// Created by Junie (AI) on 11/2/25.
//
#include "Partitioning.h"
#include "Consts.h"
#include "RunCmd.h"

#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#ifdef __unix__
#include <sys/wait.h>
#endif

static bool is_block_device(const std::string &path) {
    struct stat st{};
    if (stat(path.c_str(), &st) != 0) return false;
    return S_ISBLK(st.st_mode);
}

static std::string part_suffix(const std::string &device) {
    // For devices ending with a digit (e.g., /dev/nvme0n1, /dev/mmcblk0), use 'p'. Otherwise none.
    if (!device.empty() && std::isdigit(device.back())) return "p";
    return "";
}


bool setup_partitions(const std::string &device) {
    // Basic sanity checks
    if (device.empty()) {
        std::fprintf(stderr, "setup_partitions: empty device path provided\n");
        return false;
    }
    if (!is_block_device(device)) {
        std::fprintf(stderr, "setup_partitions: %s is not a block device or not accessible\n", device.c_str());
        return false;
    }

    std::fprintf(stderr, "WARNING: This will DESTROY all data on %s. Proceeding...\n", device.c_str());

    // Determine partition naming scheme
    std::string ps = part_suffix(device);
    std::string p1 = device + ps + "1"; // /boot (FAT32)
    std::string p2 = device + ps + "2"; // swap (4GiB)
    std::string p3 = device + ps + "3"; // root (ext4)

    // Create new GPT and partitions using parted (MiB units)
    // Layout:
    //  - 1MiB..513MiB  -> 512MiB FAT32 (ESP)
    //  - 513MiB..4609MiB -> 4096MiB swap (4GiB)
    //  - 4609MiB..100% -> ext4 root
    if (!run_cmd_with_prefix("parted -s '" + device + "' mklabel gpt", "[partition]")) return false;
    if (!run_cmd_with_prefix("parted -s '" + device + "' mkpart ESP fat32 1MiB 513MiB", "[partition]")) return false;
    if (!run_cmd_with_prefix("parted -s '" + device + "' set 1 esp on", "[partition]")) return false;
    if (!run_cmd_with_prefix("parted -s '" + device + "' mkpart swap linux-swap 513MiB 4609MiB", "[partition]")) return false;
    if (!run_cmd_with_prefix("parted -s '" + device + "' mkpart root ext4 4609MiB 100%", "[partition]")) return false;

    // Inform kernel of partition changes
    run_cmd_with_prefix("partprobe '" + device + "'", "[partition]");
    run_cmd_with_prefix("udevadm settle", "[partition]");
    // Sleep briefly to ensure nodes appear
    usleep(300 * 1000);

    // Create filesystems
    if (!run_cmd_with_prefix("mkfs.vfat -F 32 -n BOOT '" + p1 + "'", "[partition]")) return false;
    if (!run_cmd_with_prefix("mkswap -L SWAP '" + p2 + "'", "[partition]") ) return false;
    if (!run_cmd_with_prefix("swapon " + p2, "[partition]")) return false;
    if (!run_cmd_with_prefix("mkfs.ext4 -F -L ROOT '" + p3 + "'", "[partition]")) return false;

    // Verify with blkid
    bool ok = true;
    ok = ok && run_cmd_with_prefix("blkid '" + p1 + "'", "[partition]");
    ok = ok && run_cmd_with_prefix("blkid '" + p2 + "'", "[partition]");
    ok = ok && run_cmd_with_prefix("blkid '" + p3 + "'", "[partition]");

    if (ok) {
        std::fprintf(stderr, "setup_partitions: Completed successfully on %s\n", device.c_str());
    }
    return ok;
}

bool mount_partitions() {
    bool ok = true;

    // Ensure mount points exist
    std::string lfs = std::string(LFS);
    std::string boot = lfs + "/boot";

    ok = ok && run_cmd_with_prefix("mkdir -p '" + lfs + "'", "[partition]");
    if (!ok) return false;

    // If already mounted, skip mounting to keep idempotency
    bool root_mounted = run_cmd_with_prefix("mountpoint -q '" + lfs + "'", "[partition]");
    if (!root_mounted) {
        // Mount ROOT (ext4) by label
        if (!run_cmd_with_prefix("mount -t ext4 -o defaults -L ROOT '" + lfs + "'", "[partition]")) return false;
    } else {
        std::fprintf(stderr, "mount_partitions: %s already mounted\n", lfs.c_str());
    }
    ok = run_cmd_with_prefix("mkdir -p '" + boot + "'", "[partition]");
    if (!ok) return false;
    bool boot_mounted = run_cmd_with_prefix("mountpoint -q '" + boot + "'", "[partition]");
    if (!boot_mounted) {
        // Mount BOOT (vfat) by label; restrict permissions
        if (!run_cmd_with_prefix("mount -t vfat -o umask=0077 -L BOOT '" + boot + "'", "[partition]")) return false;
    } else {
        std::fprintf(stderr, "mount_partitions: %s already mounted\n", boot.c_str());
    }

    return true;
}
