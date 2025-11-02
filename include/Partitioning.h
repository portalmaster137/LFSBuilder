//
// Created by Junie (AI) on 11/2/25.
//
#pragma once

#include <string>

// WARNING: Destructive operation.
// Creates a new GPT partition table on the given block device and sets up:
//  - Partition 1: /boot, FAT32, 512 MiB (label: BOOT), with ESP flag enabled
//  - Partition 2: swap, 4 GiB (label: SWAP)
//  - Partition 3: root, ext4, remaining space (label: ROOT)
//
// The function executes standard Linux tooling: parted, partprobe/udevadm, mkfs.vfat, mkswap, mkfs.ext4.
// Returns true on success, false if any step fails. It prints diagnostic messages to stderr.
// The caller is responsible for ensuring the device is correct and not in use.
bool setup_partitions(const std::string &device);
