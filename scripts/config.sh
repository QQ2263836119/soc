#!/usr/bin/env bash
set -e
cd "$(dirname "$(readlink -f "$0")")/.."

petalinux-create -t apps -n autostart --enable --force
petalinux-create -t modules -n axi-tangxi --enable --force
# reset changes of `--force`
git restore project-spec/meta-user/recipes-apps/autostart
git clean -fd project-spec/meta-user/recipes-apps/autostart
git restore project-spec/meta-user/recipes-bsp
git clean -fd project-spec/meta-user/recipes-bsp
