#!/bin/bash
# Script outline to install and build kernel.
# Author: Siddhant Jajoo.

# ********************************************
# ssolusa
# This file requires one argument; outdir; use below default
# *********************************************


set -e
set -u


OUTDIR=/tmp/aeld
# KERNEL_REPO=git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git            # ssolusa; see next line
KERNEL_REPO=https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git            # ssolusa; per hw instruction 1.c.i.1
KERNEL_VERSION=v5.15.163
BUSYBOX_VERSION=1_33_1
FINDER_APP_DIR=$(realpath $(dirname $0))
ARCH=arm64
CROSS_COMPILE=aarch64-none-linux-gnu-


if [ $# -lt 1 ]
then
	echo "Using default directory ${OUTDIR} for output"
else
	OUTDIR=$1
	echo "Using passed directory ${OUTDIR} for output"
fi


# mkdir -p ${OUTDIR}                                                # ssolusa; removed, see next line
# ****************************************************************
# sssolusa START
# Adding per Assignment 3, Part 2, Step 1.b.
mkdir -p ${OUTDIR} || {
    echo "ERROR: Failed to create directory ${OUTDIR}" >&2
    exit 1
}
# ****************************************************************


cd "$OUTDIR"
if [ ! -d "${OUTDIR}/linux-stable" ]; then
    #Clone only if the repository does not exist.
    echo "CLONING GIT LINUX STABLE VERSION ${KERNEL_VERSION} IN ${OUTDIR}"
    git clone ${KERNEL_REPO} --depth 1 --single-branch --branch ${KERNEL_VERSION}
fi


if [ ! -e ${OUTDIR}/linux-stable/arch/${ARCH}/boot/Image ]; then
    cd linux-stable
    echo "Checking out version ${KERNEL_VERSION}"
    git checkout ${KERNEL_VERSION}
    # TODO: Add your kernel build steps here
    # *************************************************************
    # ssolusa START
    make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- mrproper      # Deep clean kernel; move .config & existing config's
    make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- defconfig     # Config virt arm dev board to simulate in QEMU
    make -j4 ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- all       # Build kernel image for booting with QEMU
    # make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- modules     # Build kernel modules; removing per HW instruction 1.e.i
    make ARCH=arm64 CROSS_COMPILE=aarch64-none-linux-gnu- dtbs          # Build the devicetree
    # *************************************************************
fi


echo "Adding the Image in $OUTDIR"                                       # ssolusa; change FROM outdir, TO $OUTDIR
sudo cp ${OUTDIR}/linux-stable/arch/arm64/boot/Image "$OUTDIR"           # ssolusa; added b/c errors; no Image


echo "Creating the staging directory for the root filesystem"
cd "$OUTDIR"
# **************************************************************************************
# ssolusa START
# Adding b/c permission errors
owner=$(stat -c '%U' "$OUTDIR")
if [ "$owner" = "root" ]; then
    echo "Changing ownership of $OUTDIR to $USER..."
    sudo chown -R $USER:$USER "$OUTDIR"
fi
# **************************************************************************************
if [ -d "${OUTDIR}/rootfs" ]
then
    echo "Deleting rootfs directory at ${OUTDIR}/rootfs and starting over"
    sudo rm -rf "$OUTDIR/rootfs"
fi


# TODO: Create necessary base directories;
# ***************************************************************************************
# ssolusa START
mkdir -p "$OUTDIR/rootfs"
cd "$OUTDIR/rootfs"
mkdir -p bin dev etc home lib lib64 proc sbin sys tmp usr var
mkdir -p usr/bin usr/lib usr/sbin
mkdir -p var/log
mkdir -p home/conf
# **************************************************************************************


cd "$OUTDIR"
if [ ! -d "${OUTDIR}/busybox" ]
then
    # git clone git://busybox.net/busybox.git                                              # ssolusa; removed, see next line
    git clone https://github.com/mirror/busybox.git                                        # ssolusa; error with above; does not have https
    cd busybox
    git checkout ${BUSYBOX_VERSION}
    # TODO:  Configure busybox
    # **********************************************************************************
    # ssolusa START
    make distclean
    make defconfig                                                                         # ls .config
    # make menuconfig                                                                      # ncurses is installed but continue to have errors
    # **********************************************************************************
else
    cd busybox
fi


# TODO: Make and install busybox
# ****************************************************************************************
# ssolusa START
make ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE}                                             # Build BusyBox
make CONFIG_PREFIX=${OUTDIR}/rootfs ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} install      # Install BusyBox
# ****************************************************************************************


echo "Library dependencies"
cd "$OUTDIR/busybox"                                                                         # ssolusa; added
${CROSS_COMPILE}readelf -a busybox | grep "program interpreter"                              # ssolusa; WAS bin/busybox
    # [Requesting program interpreter: /lib/ld-linux-aarch64.so.1]                           # ssolusa; output
${CROSS_COMPILE}readelf -a busybox | grep "Shared library"                                   # ssolusa; WAS bin/busybox
    # 0x0000000000000001 (NEEDED)             Shared library: [libm.so.6]                    # ssolusa; outputs
    # 0x0000000000000001 (NEEDED)             Shared library: [libresolv.so.2]
    # 0x0000000000000001 (NEEDED)             Shared library: [libc.so.6]



# TODO: Copy library dependencies to rootfs;
# ****************************************************************************************
# ssolusa START
cd "$OUTDIR/busybox"
ln -sf bin/busybox init                                                                      # ssolusa; added b/c errors with init
programInterpreter_List=$(${CROSS_COMPILE}readelf -a busybox | grep "program interpreter" | awk -F': ' '{print $2}' | tr -d '[]')
sharedLibrary_List=$(${CROSS_COMPILE}readelf -a busybox | grep "Shared library" | awk -F'[][]' '{print $2}')
SYSROOT=$(${CROSS_COMPILE}gcc -print-sysroot)                                                # OUTPUT: .../aarch64-none-linux-gnu/libc
for item in $programInterpreter_List; do
    echo "Program Interpreter: $item"
    cp -a "${SYSROOT}/${item}" "${OUTDIR}/rootfs/lib/"
done
for item in $sharedLibrary_List; do
    echo "Shared Library: $item"
    cp -a "${SYSROOT}/lib64/${item}" "${OUTDIR}/rootfs/lib64/"
done
# ********************************************************************************************



# TODO: Make device nodes
# ********************************************************************************************
# ssolusa START
cd "$OUTDIR/rootfs"
sudo mknod -m 622 dev/console c 5 1                                                             # was 666
sudo mknod -m 666 dev/null c 1 3
# ********************************************************************************************



# TODO: Clean and build the writer utility
# ********************************************************************************************
# ssolusa START
# FROM: finder-test.sh
cd $HOME/Documents/msGithubRepo/finder-app/
make clean
make
cp writer "$OUTDIR/rootfs/home/"                                                                 # Per HW instruction 1.e.ii
# ********************************************************************************************



# TODO: Copy the finder related scripts and executables to the /home directory on the target rootfs
# ********************************************************************************************
# ssolusa START
cd $HOME/Documents/msGithubRepo/finder-app/
cp -a finder.sh finder-test.sh "$OUTDIR/rootfs/home/"
cp -a conf/username.txt conf/assignment.txt "$OUTDIR/rootfs/home/conf/"
cp -a autorun-qemu.sh "$OUTDIR/rootfs/home/"
cp -a writer.sh "$OUTDIR/rootfs/home/"                                                           # Added b/c errors
# ********************************************************************************************



# TODO: Chown the root directory
# ********************************************************************************************
# ssolusa START
cd "$OUTDIR"
sudo chown -R $USER:$USER "$OUTDIR"                                                              # Was root:root but errors with permissions
# ********************************************************************************************



# TODO: Create initramfs.cpio.gz
# ********************************************************************************************
# ssolusa START
cd "$OUTDIR/rootfs"
find . | cpio -H newc -ov --owner root:root > "$OUTDIR/initramfs.cpio"
cd "$OUTDIR"
gzip -f initramfs.cpio
# sudo chown -R root:root "$OUTDIR"
# ********************************************************************************************



