#!/bin/bash
PACKAGE=$1
PKGNAME=$2
VERSION=$3
PKGREL=$4
ARCH=$5

echo "Name:           ${PKGNAME}"
echo "Version:        ${VERSION}"
echo "Release:        ${PKGREL}%{?dist}"
echo "Summary:        ${PKGNAME} - directory synchronizer"
echo "BuildArch:      ${ARCH}"
echo ""
echo "License:        GPL-3.0-or-later"
echo "Source:         ${PKGNAME}-${VERSION}.tar.gz"
echo ""
echo "%description"
echo "${PKGNAME} is directory synchronizer which works in symmetric and asymmetric modes"
echo ""
echo "%prep"
echo "%setup"
echo ""
echo "%install"
echo "rm -rf \$RPM_BUILD_ROOT"
echo "mkdir -p \$RPM_BUILD_ROOT"
echo "cp -r usr \$RPM_BUILD_ROOT"
echo ""
echo "%clean"
echo "rm -rf \$RPM_BUILD_ROOT"
echo ""
echo "%files"
find $PACKAGE -type f -printf "/%P\n"
echo ""
echo "%changelog"
echo "* Tue Jun 25 2024 Refolli Francesco <frefolli@campus.unimib.it> - ${VERSION}"
echo "- Created a RPM Package for this Library"
