#!/bin/sh

. ./bin/setup

export TARFILE_LOCATION
export CLONEDIR

if test -e bootstrap/download -a -e bootstrap/ooo.lst; then
    (
        cd bootstrap
        ./download ooo.lst
    )
else
    GIT_TAG=`echo $OOO_SOURCEDIRNAME | tr "a-z.-" "A-Z__"`
    tmp=`mktemp -q -d`

    cd $tmp
    wget http://cgit.freedesktop.org/libreoffice/bootstrap/plain/fetch_tarballs.sh?id=$GIT_TAG -O fetch_tarballs.sh && chmod 755 fetch_tarballs.sh
    wget http://cgit.freedesktop.org/libreoffice/bootstrap/plain/ooo.lst?id=$GIT_TAG -O ooo.lst
    ./fetch_tarballs.sh ooo.lst &&  rm -f fetch_tarballs.sh ooo.lst
    cd ..
    rmdir $tmp
fi

