#! /bin/bash

make clean
rm rel/extractor

qmake -project QT+=xml CONFIG+=debug DESTDIR=rel OBJECTS_DIR=rel LIBS="-lxerces-c -L/opt/xercesc/lib"
qmake

make


