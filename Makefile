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

test:
	meson test -C ${BUILDDIR}

install:
	mkdir -p ${DESTDIR}/usr/local/lib/
	mkdir -p ${DESTDIR}/usr/local/include/
	mkdir -p ${DESTDIR}/usr/local/share/pkgconfig
	mv builddir/libstardock.so ${DESTDIR}/usr/local/lib/
	cp -r include/* ${DESTDIR}/usr/local/include/
	cp -r stardock.pc ${DESTDIR}/usr/local/share/pkgconfig

run:
	./builddir/main.exe

docs:
	make -C ./doc
