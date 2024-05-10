#!/usr/bin/env bash
set -e
cd "$(dirname "$(readlink -f "$0")")/.."

# initial project-spec/configs/config and project-spec/configs/rootfs_config
petalinux-config --silentconfig
petalinux-config -crootfs --silentconfig

# common config
scripts/config.pl assets/configs/config project-spec/configs/config
scripts/config.pl assets/configs/config project-spec/configs/rootfs_config
# customized config
scripts/config.pl "${1:-assets/configs/example/config}" project-spec/configs/config
scripts/config.pl "${1:-assets/configs/example/config}" project-spec/configs/rootfs_config
# refresh config
petalinux-config --silentconfig
petalinux-config -crootfs --silentconfig

# config again because some option will occur after `petalinux-config --silentconfig`
scripts/config.pl assets/configs/config project-spec/configs/config
scripts/config.pl assets/configs/config project-spec/configs/rootfs_config
scripts/config.pl "${1:-assets/configs/example/config}" project-spec/configs/config
scripts/config.pl "${1:-assets/configs/example/config}" project-spec/configs/rootfs_config
petalinux-config --silentconfig
petalinux-config -crootfs --silentconfig

petalinux-create -t apps -n autostart --enable --force
petalinux-create -t modules -n axi-tangxi --enable --force
# reset changes of `--force`
git restore project-spec/meta-user/recipes-apps/autostart
git clean -fd project-spec/meta-user/recipes-apps/autostart
rm -rf project-spec/meta-user/recipes-apps/*.old
git restore project-spec/meta-user/recipes-modules
git clean -fd project-spec/meta-user/recipes-modules
rm -rf project-spec/meta-user/recipes-modules/*.old
