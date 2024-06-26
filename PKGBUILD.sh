#!/bin/bash
PACKAGE=$1
PKGNAME=$2
VERSION=$3
SHA256=$(sha256sum ${PACKAGE}.tar.gz | awk '{ print $1 }')

echo "# Contributor & Maintainer: Refolli Francesco <francesco.refolli@gmail.com>"
echo "pkgname=stardock"
echo "pkgver=${VERSION}"
echo "pkgrel=1"
echo "pkgdesc='${PKGNAME} is directory synchronizer which works in symmetric and asymmetric modes'"
echo "license=('GPL-3.0-or-later')"
echo "arch=('any')"
echo "makedepends=()"
echo "url='https://github.com/frefolli/stardock'"
echo "source=("${PACKAGE}.tar.gz")"
echo "sha256sums=('${SHA256}')"
echo "package() {"
echo "  cd \$pkgname-\$pkgver"
echo "  cp -r /usr \$pkgdir"
echo "}"
echo "options=(!debug)"
