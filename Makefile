BUILDDIR=./builddir
LIB=./builddir/stardock.so
INCLUDE=./include/stardock.hh ./include/stardock/*.hh
SRC=./src/main.cc ./src/stardock/*.cc
MESON_CONF=meson.build
BUILD_TYPE=release

@all: ${LIB}

${BUILDDIR}: ${MESON_CONF}
	meson setup --buildtype=${BUILD_TYPE} ${BUILDDIR}

${LIB}: ${BUILDDIR} ${SRC} ${INCLUDE}
	ninja -C ${BUILDDIR}

clean:
	rm -rf ${BUILDDIR}
	rm -rf target

test:
	meson test -C ${BUILDDIR}

install:
	mkdir -p ${DESTDIR}/usr/bin/
	cp builddir/main.exe ${DESTDIR}/usr/bin/stardock
	mkdir -p ${DESTDIR}/usr/lib/
	cp builddir/libstardock.so ${DESTDIR}/usr/lib/
	mkdir -p ${DESTDIR}/usr/include/
	cp -r include/* ${DESTDIR}/usr/include/
	mkdir -p ${DESTDIR}/usr/share/pkgconfig
	cp -r stardock.pc ${DESTDIR}/usr/share/pkgconfig

run:
	./builddir/main.exe

docs:
	make -C ./doc
