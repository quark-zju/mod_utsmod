#!/bin/sh

# Look up kernel headers directory in following locations:
# - Arch Linux: /usr/lib/modules/`uname -r`/build SUBDIRS=${PWD}
# - Debian: /usr/src/linux-headers-`uname -r`
# - "$KDIR"

RELEASE=`uname -r`
for i in /usr/lib/modules/$RELEASE/build /usr/src/linux-headers-$RELEASE "$KDIR"; do
  [ -d "$i" ] || continue
  exec make -C $i SUBDIRS=${PWD} modules
done

cat <<!
Kernel headers not found. Please install kernel headers first:
  - Debian: apt-get install linux-headers-\`uname -r\`
  - Arch Linux: pacman -S linux-headers 

Alternatively, you can specify a kernel header directory by
setting \`KDIR\` environment variable:
  KDIR=/path/to/kernel/headers $0
!
exit 1
