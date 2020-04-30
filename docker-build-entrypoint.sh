#!/bin/bash

VERSION_NUMBER=${VERSION_NUMBER:-1.0.0}

OUTPUT_NAME="libgit2.so.$VERSION_NUMBER.tar.gz"

cd /libgit2-src/build
cmake ..
cmake --build .
ctest -v

tar -czvf $OUTPUT_NAME ../include libgit2.so.1.0.0

cp -r $OUTPUT_NAME /build-output

