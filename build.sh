#!/bin/sh

# Try to find kernel directory, known ones:
# - Arch Linux: /usr/lib/modules/`uname -r`/build SUBDIRS=${PWD}
# - Debian: /usr/src/linux-headers-`uname -r`

RELEASE=`uname -r`
for i in /usr/lib/modules/$RELEASE/build /usr/src/linux-headers-$RELEASE; do
  [ -d $i ] || continue
  make -C $i SUBDIRS=${PWD} modules
  break
done
