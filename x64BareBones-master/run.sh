#!/bin/bash
if [ "$1" = "-d" ]
then
    qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -d int 2>&1 | grep "v=80"
else
    qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -d int 2>&1 | grep "v=80"
fi
