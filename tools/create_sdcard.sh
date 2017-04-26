#!/bin/bash

#set -x 

KERNEL=
DEVICE=

TMP_MNT_POINT=/tmp/mnt.$$
KERNEL_IMAGE_NAME=kernel.img
BOOT_FILES_DIR=$PWD/../sd_boot
BOOTCODEBIN=bootcode.bin
CONFIGTXT=config.txt
FIXUPDAT=fixup.dat
STARTELF=start.elf

function usage()
{
    echo "Usage: $0 -k <kernel_img_file> -d <sdcard_device>"
    exit 1
}

function error()
{
    local msg=$1
    echo -e "\nError: $msg. Exiting..."
    exit 2
}

function umount_device()
{
    umount ${DEVICE}*  > /dev/null 2>&1
}

function format_sdcard()
{
    echo -e "* Formating sdcard..."
    umount ${DEVICE}1  > /dev/null 2>&1
    (echo o; echo n; echo p; echo 1; echo ""; echo +100M; echo t; echo b; echo w; echo q) | fdisk ${DEVICE} > /dev/null 2>&1
    mkfs.vfat ${DEVICE}1 > /dev/null 2>&1
    sync
}

function check_files_have_been_copied()
{
     [ ! -f ${TMP_MNT_POINT}/${BOOTCODEBIN} ] && error "${TMP_MNT_POINT}/${BOOTCODEBIN} does not exists"
     [ ! -f ${TMP_MNT_POINT}/${CONFIGTXT} ] && error "${TMP_MNT_POINT}/${CONFIGTXT} does not exists"
     [ ! -f ${TMP_MNT_POINT}/${FIXUPDAT} ] && error "${TMP_MNT_POINT}/${FIXUPDATA} does not exists"
     [ ! -f ${TMP_MNT_POINT}/${STARTELF} ] && error "${TMP_MNT_POINT}/${STARTELF} does not exists"
     [ ! -f ${TMP_MNT_POINT}/${KERNEL_IMAGE_NAME} ] && error "${TMP_MNT_POINT}/${KERNEL_IMAGE_NAME} does not exists"
}

function copy_files_to_sdcard()
{
    echo -e "* Copying files to sdcard..."
    [ ! -d ${TMP_MNT_POINT} ] && mkdir -p ${TMP_MNT_POINT}
    mount -t vfat ${DEVICE}1 ${TMP_MNT_POINT}
    cp ${BOOT_FILES_DIR}/${BOOTCODEBIN} ${TMP_MNT_POINT}
    cp ${BOOT_FILES_DIR}/${CONFIGTXT} ${TMP_MNT_POINT}
    cp ${BOOT_FILES_DIR}/${FIXUPDAT} ${TMP_MNT_POINT}
    cp ${BOOT_FILES_DIR}/${STARTELF} ${TMP_MNT_POINT}
    cp ${KERNEL} ${TMP_MNT_POINT}
    sync
    check_files_have_been_copied
    umount ${TMP_MNT_POINT} > /dev/null 2>&1
}

function clean_temporals()
{
    echo -e "* Removing temporals..."
    [ -d ${TMP_MNT_POINT} ] && rmdir ${TMP_MNT_POINT}
}

# MAIN

# Check root
[ ! `id -u` -eq 0 ] && error "You must exec this as root"

# Program options
while getopts ":k:d:" o; do
    case "${o}" in
        k)
            KERNEL=${OPTARG}
            ;;
        d)
            DEVICE=${OPTARG}
            ;;
        *)
            usage
            ;; 
    esac
done

shift $((OPTIND-1))

[ -z "${KERNEL}" ] || [ -z "${DEVICE}" ] && usage

echo ""
echo "Welcome to $0 script to generate a sdcard to be able to run MSKE kernel in Raspberry Pi..."
echo ""
echo "The following configuration has been entered..."
echo " - kernel_image = ${KERNEL}"
echo " - sd_device = ${DEVICE}"
echo ""
echo "Data in your ${DEVICE} will be lost..."

read -n 1 -s -p "Do you want to continue? (y/n)" key

while [ "${key}" != "y" ] ; do
    [ "$key" = "n" ] && echo -e "\n* Exiting..." && exit 1
    echo ""
    read -n 1 -s -p "* Is this correct? (y/n)" key
done

echo ""

umount_device
format_sdcard
copy_files_to_sdcard
clean_temporals

echo ""
echo "You should be able to start your Raspberry with the generated sdcard..."
