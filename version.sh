#!/bin/bash

version=$(git describe --long --match "version_*")
regex='version_([0-9]+)\.([0-9]+)\.([0-9]+)(-[0-9]+)?(-.*)?'

[[ "$version" =~ $regex ]]

major="${BASH_REMATCH[1]}"
minor="${BASH_REMATCH[2]}"
patch="${BASH_REMATCH[3]}"
build="${BASH_REMATCH[4]:1}"
hash="${BASH_REMATCH[5]:1}"

echo "v.$major.$minor.$patch-$build-$hash"

cat > src/version.cpp <<- EOM
#include <version.h>

const char *version = "$major.$minor.$patch-$build-$hash";

EOM
