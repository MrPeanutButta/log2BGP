#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++-4.8
CXX=g++-4.8
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cli.o \
	${OBJECTDIR}/cli_main.o \
	${OBJECTDIR}/cmdparser.o \
	${OBJECTDIR}/misc.o \
	${OBJECTDIR}/tcp_client.o \
	${OBJECTDIR}/util.o \
	${OBJECTDIR}/util_bgp.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fpermissive
CXXFLAGS=-fpermissive

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lreadline -lutil -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplg-cli

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplg-cli: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplg-cli ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/cli.o: nbproject/Makefile-${CND_CONF}.mk cli.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DTCP_CLIENT_CLI -DUSE_CLI -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cli.o cli.cpp

${OBJECTDIR}/cli_main.o: nbproject/Makefile-${CND_CONF}.mk cli_main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DTCP_CLIENT_CLI -DUSE_CLI -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cli_main.o cli_main.cpp

${OBJECTDIR}/cmdparser.o: nbproject/Makefile-${CND_CONF}.mk cmdparser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DTCP_CLIENT_CLI -DUSE_CLI -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmdparser.o cmdparser.cpp

${OBJECTDIR}/misc.o: nbproject/Makefile-${CND_CONF}.mk misc.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DTCP_CLIENT_CLI -DUSE_CLI -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/misc.o misc.cpp

${OBJECTDIR}/tcp_client.o: nbproject/Makefile-${CND_CONF}.mk tcp_client.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DTCP_CLIENT_CLI -DUSE_CLI -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tcp_client.o tcp_client.cpp

${OBJECTDIR}/util.o: nbproject/Makefile-${CND_CONF}.mk util.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DTCP_CLIENT_CLI -DUSE_CLI -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/util.o util.cpp

${OBJECTDIR}/util_bgp.o: nbproject/Makefile-${CND_CONF}.mk util_bgp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -DTCP_CLIENT_CLI -DUSE_CLI -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/util_bgp.o util_bgp.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplg-cli

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
