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
	${OBJECTDIR}/bgp_adj_rib_wdrawn.o \
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

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-g3
CXXFLAGS=-g3

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=--64

# Link Libraries and Options
LDLIBSOPTIONS=-lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplgd

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplgd: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplgd ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/api/bgp_api.o: api/bgp_api.cpp 
	${MKDIR} -p ${OBJECTDIR}/api
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/api/bgp_api.o api/bgp_api.cpp

${OBJECTDIR}/bgp.o: bgp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp.o bgp.cpp

${OBJECTDIR}/bgp_adj_rib_wdrawn.o: bgp_adj_rib_wdrawn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_adj_rib_wdrawn.o bgp_adj_rib_wdrawn.cpp

${OBJECTDIR}/bgp_attrib_alias.o: bgp_attrib_alias.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_attrib_alias.o bgp_attrib_alias.cpp

${OBJECTDIR}/bgp_database.o: bgp_database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_database.o bgp_database.cpp

${OBJECTDIR}/bgp_main.o: bgp_main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_main.o bgp_main.cpp

${OBJECTDIR}/bgp_notification.o: bgp_notification.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_notification.o bgp_notification.cpp

${OBJECTDIR}/bgp_open.o: bgp_open.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_open.o bgp_open.cpp

${OBJECTDIR}/bgp_peer.o: bgp_peer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_peer.o bgp_peer.cpp

${OBJECTDIR}/bgp_send.o: bgp_send.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_send.o bgp_send.cpp

${OBJECTDIR}/bgp_syslog.o: bgp_syslog.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_syslog.o bgp_syslog.cpp

${OBJECTDIR}/bgp_thread-mgr.o: bgp_thread-mgr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_thread-mgr.o bgp_thread-mgr.cpp

${OBJECTDIR}/bgp_timer.o: bgp_timer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_timer.o bgp_timer.cpp

${OBJECTDIR}/bgp_update.o: bgp_update.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_update.o bgp_update.cpp

${OBJECTDIR}/cli/bgp_cli.o: cli/bgp_cli.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/cli/bgp_cli.o cli/bgp_cli.cpp

${OBJECTDIR}/cli/bgp_cmdparser.o: cli/bgp_cmdparser.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/cli/bgp_cmdparser.o cli/bgp_cmdparser.cpp

${OBJECTDIR}/misc.o: misc.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/misc.o misc.cpp

${OBJECTDIR}/tcp_client.o: tcp_client.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/tcp_client.o tcp_client.cpp

${OBJECTDIR}/tcp_server.o: tcp_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -MMD -MP -MF $@.d -o ${OBJECTDIR}/tcp_server.o tcp_server.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-conf ${TESTFILES}
${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/bgp_api_runner.o ${TESTDIR}/tests/bgp_apil_test.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc}   -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS} `cppunit-config --libs`   


${TESTDIR}/tests/bgp_api_runner.o: tests/bgp_api_runner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/bgp_api_runner.o tests/bgp_api_runner.cpp


${TESTDIR}/tests/bgp_apil_test.o: tests/bgp_apil_test.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} $@.d
	$(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 `cppunit-config --cflags` -MMD -MP -MF $@.d -o ${TESTDIR}/tests/bgp_apil_test.o tests/bgp_apil_test.cpp


${OBJECTDIR}/api/bgp_api_nomain.o: ${OBJECTDIR}/api/bgp_api.o api/bgp_api.cpp 
	${MKDIR} -p ${OBJECTDIR}/api
	@NMOUTPUT=`${NM} ${OBJECTDIR}/api/bgp_api.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/api/bgp_api_nomain.o api/bgp_api.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/api/bgp_api.o ${OBJECTDIR}/api/bgp_api_nomain.o;\
	fi

${OBJECTDIR}/bgp_nomain.o: ${OBJECTDIR}/bgp.o bgp.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_nomain.o bgp.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp.o ${OBJECTDIR}/bgp_nomain.o;\
	fi

${OBJECTDIR}/bgp_adj_rib_wdrawn_nomain.o: ${OBJECTDIR}/bgp_adj_rib_wdrawn.o bgp_adj_rib_wdrawn.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_adj_rib_wdrawn.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_adj_rib_wdrawn_nomain.o bgp_adj_rib_wdrawn.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_adj_rib_wdrawn.o ${OBJECTDIR}/bgp_adj_rib_wdrawn_nomain.o;\
	fi

${OBJECTDIR}/bgp_attrib_alias_nomain.o: ${OBJECTDIR}/bgp_attrib_alias.o bgp_attrib_alias.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_attrib_alias.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_attrib_alias_nomain.o bgp_attrib_alias.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_attrib_alias.o ${OBJECTDIR}/bgp_attrib_alias_nomain.o;\
	fi

${OBJECTDIR}/bgp_database_nomain.o: ${OBJECTDIR}/bgp_database.o bgp_database.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_database.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_database_nomain.o bgp_database.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_database.o ${OBJECTDIR}/bgp_database_nomain.o;\
	fi

${OBJECTDIR}/bgp_main_nomain.o: ${OBJECTDIR}/bgp_main.o bgp_main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_main_nomain.o bgp_main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_main.o ${OBJECTDIR}/bgp_main_nomain.o;\
	fi

${OBJECTDIR}/bgp_notification_nomain.o: ${OBJECTDIR}/bgp_notification.o bgp_notification.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_notification.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_notification_nomain.o bgp_notification.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_notification.o ${OBJECTDIR}/bgp_notification_nomain.o;\
	fi

${OBJECTDIR}/bgp_open_nomain.o: ${OBJECTDIR}/bgp_open.o bgp_open.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_open.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_open_nomain.o bgp_open.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_open.o ${OBJECTDIR}/bgp_open_nomain.o;\
	fi

${OBJECTDIR}/bgp_peer_nomain.o: ${OBJECTDIR}/bgp_peer.o bgp_peer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_peer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_peer_nomain.o bgp_peer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_peer.o ${OBJECTDIR}/bgp_peer_nomain.o;\
	fi

${OBJECTDIR}/bgp_send_nomain.o: ${OBJECTDIR}/bgp_send.o bgp_send.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_send.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_send_nomain.o bgp_send.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_send.o ${OBJECTDIR}/bgp_send_nomain.o;\
	fi

${OBJECTDIR}/bgp_syslog_nomain.o: ${OBJECTDIR}/bgp_syslog.o bgp_syslog.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_syslog.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_syslog_nomain.o bgp_syslog.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_syslog.o ${OBJECTDIR}/bgp_syslog_nomain.o;\
	fi

${OBJECTDIR}/bgp_thread-mgr_nomain.o: ${OBJECTDIR}/bgp_thread-mgr.o bgp_thread-mgr.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_thread-mgr.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_thread-mgr_nomain.o bgp_thread-mgr.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_thread-mgr.o ${OBJECTDIR}/bgp_thread-mgr_nomain.o;\
	fi

${OBJECTDIR}/bgp_timer_nomain.o: ${OBJECTDIR}/bgp_timer.o bgp_timer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_timer.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_timer_nomain.o bgp_timer.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_timer.o ${OBJECTDIR}/bgp_timer_nomain.o;\
	fi

${OBJECTDIR}/bgp_update_nomain.o: ${OBJECTDIR}/bgp_update.o bgp_update.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/bgp_update.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/bgp_update_nomain.o bgp_update.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/bgp_update.o ${OBJECTDIR}/bgp_update_nomain.o;\
	fi

${OBJECTDIR}/cli/bgp_cli_nomain.o: ${OBJECTDIR}/cli/bgp_cli.o cli/bgp_cli.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cli/bgp_cli.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cli/bgp_cli_nomain.o cli/bgp_cli.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cli/bgp_cli.o ${OBJECTDIR}/cli/bgp_cli_nomain.o;\
	fi

${OBJECTDIR}/cli/bgp_cmdparser_nomain.o: ${OBJECTDIR}/cli/bgp_cmdparser.o cli/bgp_cmdparser.cpp 
	${MKDIR} -p ${OBJECTDIR}/cli
	@NMOUTPUT=`${NM} ${OBJECTDIR}/cli/bgp_cmdparser.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/cli/bgp_cmdparser_nomain.o cli/bgp_cmdparser.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/cli/bgp_cmdparser.o ${OBJECTDIR}/cli/bgp_cmdparser_nomain.o;\
	fi

${OBJECTDIR}/misc_nomain.o: ${OBJECTDIR}/misc.o misc.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/misc.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/misc_nomain.o misc.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/misc.o ${OBJECTDIR}/misc_nomain.o;\
	fi

${OBJECTDIR}/tcp_client_nomain.o: ${OBJECTDIR}/tcp_client.o tcp_client.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/tcp_client.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/tcp_client_nomain.o tcp_client.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/tcp_client.o ${OBJECTDIR}/tcp_client_nomain.o;\
	fi

${OBJECTDIR}/tcp_server_nomain.o: ${OBJECTDIR}/tcp_server.o tcp_server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/tcp_server.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} $@.d;\
	    $(COMPILE.cc) -g -Werror -DBGP_GDB_DEBUG_NO_DAEMON -DBGP_GDB_DEBUG_RIB_PROFILE -DTCP_CLIENT_BGPD -DUSE_BGPD -Iinclude/cli -Iinclude -std=c++11 -Dmain=__nomain -MMD -MP -MF $@.d -o ${OBJECTDIR}/tcp_server_nomain.o tcp_server.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/tcp_server.o ${OBJECTDIR}/tcp_server_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/bigplgd

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
