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
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/api/bgp_api.o \
	${OBJECTDIR}/cli/bgp_cli.o \
	${OBJECTDIR}/cli/bgp_cmdparser.o \
	${OBJECTDIR}/src/bgp.o \
	${OBJECTDIR}/src/bgp_attrib_alias.o \
	${OBJECTDIR}/src/bgp_database.o \
	${OBJECTDIR}/src/bgp_main.o \
	${OBJECTDIR}/src/bgp_notification.o \
	${OBJECTDIR}/src/bgp_open.o \
	${OBJECTDIR}/src/bgp_peer.o \
	${OBJECTDIR}/src/bgp_send.o \
	${OBJECTDIR}/src/bgp_syslog.o \
	${OBJECTDIR}/src/bgp_thread-mgr.o \
	${OBJECTDIR}/src/bgp_timer.o \
	${OBJECTDIR}/src/bgp_update.o \
	${OBJECTDIR}/src/misc.o \
	${OBJECTDIR}/src/tcp_client.o \
	${OBJECTDIR}/src/tcp_server.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/log2bgp

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/log2bgp: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/log2bgp ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/api/bgp_api.o: api/bgp_api.cpp 
	${MKDIR} -p ${OBJECTDIR}/api
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/api/bgp_api.o api/bgp_api.cpp

${OBJECTDIR}/cli/bgp_cli.o: cli/bgp_cli.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cli/bgp_cli.o cli/bgp_cli.cpp

${OBJECTDIR}/cli/bgp_cmdparser.o: cli/bgp_cmdparser.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cli/bgp_cmdparser.o cli/bgp_cmdparser.cpp

${OBJECTDIR}/src/bgp.o: src/bgp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp.o src/bgp.cpp

${OBJECTDIR}/src/bgp_attrib_alias.o: src/bgp_attrib_alias.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_attrib_alias.o src/bgp_attrib_alias.cpp

${OBJECTDIR}/src/bgp_database.o: src/bgp_database.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_database.o src/bgp_database.cpp

${OBJECTDIR}/src/bgp_main.o: src/bgp_main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_main.o src/bgp_main.cpp

${OBJECTDIR}/src/bgp_notification.o: src/bgp_notification.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_notification.o src/bgp_notification.cpp

${OBJECTDIR}/src/bgp_open.o: src/bgp_open.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_open.o src/bgp_open.cpp

${OBJECTDIR}/src/bgp_peer.o: src/bgp_peer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_peer.o src/bgp_peer.cpp

${OBJECTDIR}/src/bgp_send.o: src/bgp_send.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_send.o src/bgp_send.cpp

${OBJECTDIR}/src/bgp_syslog.o: src/bgp_syslog.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_syslog.o src/bgp_syslog.cpp

${OBJECTDIR}/src/bgp_thread-mgr.o: src/bgp_thread-mgr.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_thread-mgr.o src/bgp_thread-mgr.cpp

${OBJECTDIR}/src/bgp_timer.o: src/bgp_timer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_timer.o src/bgp_timer.cpp

${OBJECTDIR}/src/bgp_update.o: src/bgp_update.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bgp_update.o src/bgp_update.cpp

${OBJECTDIR}/src/misc.o: src/misc.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/misc.o src/misc.cpp

${OBJECTDIR}/src/tcp_client.o: src/tcp_client.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tcp_client.o src/tcp_client.cpp

${OBJECTDIR}/src/tcp_server.o: src/tcp_server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tcp_server.o src/tcp_server.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/log2bgp

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
