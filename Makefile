WIN32_CLEAR_ENV=unset $$(env | grep -vi '^EDITOR=\|^HOME=\|^LANG=\|MXE\|^PATH=' | grep -vi 'PKG_CONFIG\|PROXY\|^PS1=\|^TERM=' | cut -d '=' -f1 | tr '\n' ' ')
WIN32_MXE=/home/bin/mxe_cross_compiler
WIN32_QMAKE=${WIN32_MXE}/usr/i686-w64-mingw32.static/qt5/bin/qmake
WIN32_PATH=${WIN32_MXE}/usr/bin/

LINUX_QMAKE32=/home/bin/qt-32-dyn/5.4/gcc/bin/qmake
LINUX_QMAKE64=/usr/bin/qmake

LINUX_LRELEASE=/usr/bin/lrelease

VERSION=$$( git describe --tags )


all: re_typeset_win32_static_zip re_typeset_linux32_tgz re_typeset_linux64_tgz

clean:
	-rm -r build-${VERSION}/*

clean_all:
	-rm -r build-*

dirs:
	mkdir -p build-${VERSION}/re-typeset-win32-static-build
	mkdir -p build-${VERSION}/re-typeset-win32-static-${VERSION}/res
	mkdir -p build-${VERSION}/re-typeset-linux32-build
	mkdir -p build-${VERSION}/re-typeset-linux32-${VERSION}/res
	mkdir -p build-${VERSION}/re-typeset-linux64-build
	mkdir -p build-${VERSION}/re-typeset-linux64-${VERSION}/res

languages: dirs
	${LINUX_LRELEASE} re-typeset-src/re-typeset.pro


#---------------------------
re_typeset_win32_static_zip: re_typeset_win32_static
	cd build-${VERSION} \
		&& zip -r re-typeset-win32-static-${VERSION}.zip re-typeset-win32-static-${VERSION}

re_typeset_win32_static_copy: languages
	cp re-typeset-src/lang/*.qm build-${VERSION}/re-typeset-win32-static-${VERSION}/res/
	cp -r re-typeset-src/img/* build-${VERSION}/re-typeset-win32-static-${VERSION}/res/
	cat re-typeset-src/LICENSE.TXT | sed -e 's/$$/\r/g' > build-${VERSION}/re-typeset-win32-static-${VERSION}/LICENSE.TXT
	cat re-typeset-src/version.hpp | grep -v '#' | sed -e 's!//!!g'| sed -e 's/$$/\r/g' > build-${VERSION}/re-typeset-win32-static-${VERSION}/RELEASENOTES.TXT

re_typeset_win32_static: dirs re_typeset_win32_static_copy
	${WIN32_CLEAR_ENV} \
		&& export PATH=${WIN32_PATH}:$$PATH \
		&& cd build-${VERSION}/re-typeset-win32-static-build \
		&& ${WIN32_QMAKE} DEFINES+='COMPILATION=\"\\\""windows x86_32, static_lib"\\\"\"' \
		-spec win32-g++ -Wall -o Makefile ../../re-typeset-src/re-typeset.pro \
		&& make \
		&& cp release/re-typeset.exe ../re-typeset-win32-static-${VERSION}/

#---------------------------
re_typeset_linux32_tgz: re_typeset_linux32
	cd build-${VERSION} \
		&& tar -cvzf re-typeset-linux32-${VERSION}.tgz re-typeset-linux32-${VERSION}

re_typeset_linux32_copy: languages
	cp re-typeset-src/lang/*.qm build-${VERSION}/re-typeset-linux32-${VERSION}/res/
	cp -r re-typeset-src/img/* build-${VERSION}/re-typeset-linux32-${VERSION}/res/
	cp re-typeset-src/LICENSE.TXT build-${VERSION}/re-typeset-linux32-${VERSION}/
	cat re-typeset-src/version.hpp | grep -v '#' | sed -e 's!//!!g' > build-${VERSION}/re-typeset-linux32-${VERSION}/RELEASENOTES.TXT

re_typeset_linux32: dirs re_typeset_linux32_copy
	cd build-${VERSION}/re-typeset-linux32-build \
		&& ${LINUX_QMAKE32} DEFINES+='COMPILATION=\"\\\""linux x86_32"\\\"\"' \
		-spec linux-g++-32 -Wall -o Makefile ../../re-typeset-src/re-typeset.pro \
		&& make \
		&& cp re-typeset ../re-typeset-linux32-${VERSION}/


#---------------------------
re_typeset_linux64_tgz: re_typeset_linux64
	cd build-${VERSION} \
		&& tar -cvzf re-typeset-linux64-${VERSION}.tgz re-typeset-linux64-${VERSION}

re_typeset_linux64_copy: languages
	cp re-typeset-src/lang/*.qm build-${VERSION}/re-typeset-linux64-${VERSION}/res/
	cp -r re-typeset-src/img/* build-${VERSION}/re-typeset-linux64-${VERSION}/res/
	cp re-typeset-src/LICENSE.TXT build-${VERSION}/re-typeset-linux64-${VERSION}/
	cat re-typeset-src/version.hpp | grep -v '#' | sed -e 's!//!!g' > build-${VERSION}/re-typeset-linux64-${VERSION}/RELEASENOTES.TXT

re_typeset_linux64: dirs re_typeset_linux64_copy
	cd build-${VERSION}/re-typeset-linux64-build \
		&& ${LINUX_QMAKE64} DEFINES+='COMPILATION=\"\\\""linux x86_64"\\\"\"' \
		-spec linux-g++-64 -Wall -o Makefile ../../re-typeset-src/re-typeset.pro \
		&& make \
		&& cp re-typeset ../re-typeset-linux64-${VERSION}/


#---------------------------
mxe_cross_compiler: 
	cd ${WIN32_MXE} && make gcc gcc-gmp gcc-mpc gcc-mpfr qt qtbase qtimageformats qt5 # kompilcja MXE (http://mxe.cc)

