#!/bin/bash
PACKAGE=$1
PKGNAME=$2
VERSION=$3
PKGREL=$4
ARCH=$5
SHA256=$(sha256sum ${PACKAGE}.tar.gz | awk '{ print $1 }')

echo "# Contributor & Maintainer: Refolli Francesco <francesco.refolli@gmail.com>"
echo "pkgname=${PKGNAME}"
echo "pkgver=${VERSION}"
echo "pkgrel=${PKGREL}"
echo "pkgdesc='${PKGNAME} is directory synchronizer which works in symmetric and asymmetric modes'"
echo "license=('GPL-3.0-or-later')"
echo "arch=('${ARCH}')"
echo "makedepends=()"
echo "url='https://github.com/frefolli/stardock'"
echo "source=("${PACKAGE}.tar.gz")"
echo "sha256sums=('${SHA256}')"
echo "package() {"
echo "  cd \$pkgname-\$pkgver"
echo "  cp -r usr \$pkgdir"
echo "}"
echo "options=(!debug)"
