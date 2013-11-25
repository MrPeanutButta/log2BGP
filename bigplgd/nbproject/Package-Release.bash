#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build
CND_DLIB_EXT=so
NBTMPDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplgd
OUTPUT_BASENAME=bigplgd
PACKAGE_TOP_DIR=/usr/sbin/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
rm -rf ${NBTMPDIR}
mkdir -p ${NBTMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/sbin"
copyFileToTmpDir "${OUTPUT_PATH}" "${NBTMPDIR}//usr/sbin/${OUTPUT_BASENAME}" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/sbin"
copyFileToTmpDir "../cli/dist/Debug/GNU-Linux-x86/bigplg-cli" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bigplg-cli" 0755

cd "${TOP}"
makeDirectory "${NBTMPDIR}//usr/sbin"
copyFileToTmpDir "api/bigplg-api.rb" "${NBTMPDIR}/${PACKAGE_TOP_DIR}bigplg-api" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//etc/bigplgd"
copyFileToTmpDir "etc/bgpd.conf" "${NBTMPDIR}//etc/${OUTPUT_BASENAME}/bigplgd.conf" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//etc/bigplgd"
copyFileToTmpDir "../LICENSE" "${NBTMPDIR}//etc/${OUTPUT_BASENAME}/LICENSE" 0644

cd "${TOP}"
makeDirectory "${NBTMPDIR}//etc/bigplgd"
copyFileToTmpDir "../README.md" "${NBTMPDIR}//etc/${OUTPUT_BASENAME}/README.md" 0644


# Create control file
cd "${TOP}"
CONTROL_FILE=${NBTMPDIR}/DEBIAN/control
rm -f ${CONTROL_FILE}
mkdir -p ${NBTMPDIR}/DEBIAN

cd "${TOP}"
echo 'Package: bigplg' >> ${CONTROL_FILE}
echo 'Version: 1.0.3' >> ${CONTROL_FILE}
echo 'Architecture: amd64' >> ${CONTROL_FILE}
echo 'Maintainer: aaron.hebert@gmail.com' >> ${CONTROL_FILE}
echo 'Description: ...Border Gateway Protocol: A transparent looking-glass (BigPlg)' >> ${CONTROL_FILE}

# Create Debian Package
cd "${TOP}"
cd "${NBTMPDIR}/.."
dpkg-deb  --build ${TMPDIRNAME}
checkReturnCode
cd "${TOP}"
mkdir -p  ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package
mv ${NBTMPDIR}.deb ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/bigplg-1.0.3-${CND_PLATFORM}_64.deb
checkReturnCode
echo Debian: ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/package/bigplg-1.0.3-${CND_PLATFORM}_64.deb

# Cleanup
cd "${TOP}"
rm -rf ${NBTMPDIR}
