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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/api/bgp_api.o \
	${OBJECTDIR}/bgp.o \
	${OBJECTDIR}/bgp_attrib_alias.o \
	${OBJECTDIR}/bgp_database.o \
	${OBJECTDIR}/bgp_main.o \
	${OBJECTDIR}/bgp_notification.o \
	${OBJECTDIR}/bgp_open.o \
	${OBJECTDIR}/bgp_peer.o \
	${OBJECTDIR}/bgp_send.o \
	${OBJECTDIR}/bgp_syslog.o \
	${OBJECTDIR}/bgp_thread-mgr.o \
	${OBJECTDIR}/bgp_timer.o \
	${OBJECTDIR}/bgp_update.o \
	${OBJECTDIR}/cli/bgp_cli.o \
	${OBJECTDIR}/cli/bgp_cmdparser.o \
	${OBJECTDIR}/misc.o \
	${OBJECTDIR}/tcp_client.o \
	${OBJECTDIR}/tcp_server.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64
CXXFLAGS=-m64

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/log2bgp

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/log2bgp: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/log2bgp ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/api/bgp_api.o: nbproject/Makefile-${CND_CONF}.mk api/bgp_api.cpp 
	${MKDIR} -p ${OBJECTDIR}/api
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/api/bgp_api.o api/bgp_api.cpp

${OBJECTDIR}/bgp.o: nbproject/Makefile-${CND_CONF}.mk bgp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp.o bgp.cpp

${OBJECTDIR}/bgp_attrib_alias.o: nbproject/Makefile-${CND_CONF}.mk bgp_attrib_alias.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_attrib_alias.o bgp_attrib_alias.cpp

${OBJECTDIR}/bgp_database.o: nbproject/Makefile-${CND_CONF}.mk bgp_database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_database.o bgp_database.cpp

${OBJECTDIR}/bgp_main.o: nbproject/Makefile-${CND_CONF}.mk bgp_main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_main.o bgp_main.cpp

${OBJECTDIR}/bgp_notification.o: nbproject/Makefile-${CND_CONF}.mk bgp_notification.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_notification.o bgp_notification.cpp

${OBJECTDIR}/bgp_open.o: nbproject/Makefile-${CND_CONF}.mk bgp_open.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_open.o bgp_open.cpp

${OBJECTDIR}/bgp_peer.o: nbproject/Makefile-${CND_CONF}.mk bgp_peer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_peer.o bgp_peer.cpp

${OBJECTDIR}/bgp_send.o: nbproject/Makefile-${CND_CONF}.mk bgp_send.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_send.o bgp_send.cpp

${OBJECTDIR}/bgp_syslog.o: nbproject/Makefile-${CND_CONF}.mk bgp_syslog.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_syslog.o bgp_syslog.cpp

${OBJECTDIR}/bgp_thread-mgr.o: nbproject/Makefile-${CND_CONF}.mk bgp_thread-mgr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_thread-mgr.o bgp_thread-mgr.cpp

${OBJECTDIR}/bgp_timer.o: nbproject/Makefile-${CND_CONF}.mk bgp_timer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_timer.o bgp_timer.cpp

${OBJECTDIR}/bgp_update.o: nbproject/Makefile-${CND_CONF}.mk bgp_update.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/bgp_update.o bgp_update.cpp

${OBJECTDIR}/cli/bgp_cli.o: nbproject/Makefile-${CND_CONF}.mk cli/bgp_cli.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cli/bgp_cli.o cli/bgp_cli.cpp

${OBJECTDIR}/cli/bgp_cmdparser.o: nbproject/Makefile-${CND_CONF}.mk cli/bgp_cmdparser.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cli/bgp_cmdparser.o cli/bgp_cmdparser.cpp

${OBJECTDIR}/misc.o: nbproject/Makefile-${CND_CONF}.mk misc.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/misc.o misc.cpp

${OBJECTDIR}/tcp_client.o: nbproject/Makefile-${CND_CONF}.mk tcp_client.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tcp_client.o tcp_client.cpp

${OBJECTDIR}/tcp_server.o: nbproject/Makefile-${CND_CONF}.mk tcp_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tcp_server.o tcp_server.cpp

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
