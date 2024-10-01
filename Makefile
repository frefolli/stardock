BUILDDIR=./builddir
LIB=./builddir/libstardock.so
EXE=./builddir/main.exe
INCLUDE=./include/stardock.hh ./include/stardock/*.hh
SRC=./src/main.cc ./src/stardock/*.cc
MESON_CONF=meson.build
BUILD_TYPE=release

@all: ${LIB} ${EXE} docs

${BUILDDIR}: ${MESON_CONF}
	meson setup --buildtype=${BUILD_TYPE} ${BUILDDIR}

${LIB}: ${BUILDDIR} ${SRC} ${INCLUDE}
	ninja -C ${BUILDDIR}

clean:
	rm -rf ${BUILDDIR}
	rm -rf target
	rm -rf doc/html
	rm -rf doc/latex

test:
	meson test -C ${BUILDDIR}

install: ${LIB} ${EXE} doc
	mkdir -p ${DESTDIR}/usr/bin/
	cp ${EXE} ${DESTDIR}/usr/bin/stardock
	mkdir -p ${DESTDIR}/usr/lib64/
	cp ${LIB} ${DESTDIR}/usr/lib64/
	mkdir -p ${DESTDIR}/usr/include/
	cp -r include/* ${DESTDIR}/usr/include/
	mkdir -p ${DESTDIR}/usr/share/pkgconfig
	cp -r stardock.pc ${DESTDIR}/usr/share/pkgconfig
	mkdir -p ${DESTDIR}/usr/share/doc/stardock
	cp -r doc/html ${DESTDIR}/usr/share/doc/stardock/html

run:
	./builddir/main.exe

docs:
	make -C ./doc
