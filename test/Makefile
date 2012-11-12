# $Header: /space/home/eng/cjm/cvs/libdprt-ccs/test/Makefile,v 0.16 2012-11-12 16:24:39 cjm Exp $

include	../../../Makefile.common
include ../../Makefile.common
include ../Makefile.common

LIBNAME		= $(LIBDPRT_HOME)_$(CCS_HOME)
INCDIR 		= $(LIBDPRT_CCS_SRC_HOME)/include
DOCSDIR 	= $(LIBDPRT_CCS_DOC_HOME)/test
DOCFLAGS 	= -static
BINDIR		= $(LIBDPRT_CCS_BIN_HOME)/test/${HOSTTYPE}
OBJECT_CFLAGS	= $(LIBDPRT_SRC_HOME)/object/include
# log_udp library (log_udp.h is included for verbosity settings)
LOG_UDP_HOME	= log_udp
LOG_UDP_SRC_HOME= $(LT_SRC_HOME)/$(LOG_UDP_HOME)
LOG_UDP_CFLAGS	= -I$(LOG_UDP_SRC_HOME)/include
CFLAGS 		= -g -I$(INCDIR) -I$(JNIINCDIR) -I$(JNIMDINCDIR) -I$(JNIGENERALINCDIR) -I$(OBJECT_CFLAGS) \
		$(LOG_UDP_CFLAGS)

SRCS 		= dprt_test.c
OBJS 		= $(SRCS:%.c=$(BINDIR)/%.o)
DOCS 		= $(SRCS:%.c=$(DOCSDIR)/%.html)

top: ${BINDIR}/dprt_test docs

${BINDIR}/dprt_test: $(BINDIR)/dprt_test.o $(LT_LIB_HOME)/$(LIBNAME).so
	$(CC) -o $@ $(BINDIR)/dprt_test.o -L$(LT_LIB_HOME) -ldprt_ccs -ldprt_object -ldprt_jni_general $(TIMELIB) -lm -lc

$(BINDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

docs: $(DOCS)

$(DOCS) : $(SRCS)
	-$(CDOC) -d $(DOCSDIR) -h $(INCDIR) $(DOCFLAGS) $(SRCS)

depend:
	makedepend -p$(BINDIR)/ -- $(CFLAGS) -- $(SRCS)

clean:
	-$(RM) $(RM_OPTIONS) ${BINDIR}/dprt_test $(OBJS) $(TIDY_OPTIONS)

tidy:
	-$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)

backup: tidy
	-$(RM) $(RM_OPTIONS) $(LIBDPRT_BIN_HOME)/test/dprt_test

checkin:
	-$(CI) $(CI_OPTIONS) $(SRCS)

checkout:
	$(CO) $(CO_OPTIONS) $(SRCS)

#
# $Log: not supported by cvs2svn $
# Revision 0.15  2008/10/07 13:59:06  cjm
# Added OBJECT_CFLAGS so we can enable object logging in the test code.
#
# Revision 0.14  2004/03/31 16:49:08  cjm
# Added -I$(JNIINCDIR) -I$(JNIMDINCDIR), as jni general now includes
# jni.h.
#
# Revision 0.13  2004/03/31 16:46:59  cjm
# Removed general JNI code and put into dprt_jni_general.
#
# Revision 0.12  2004/03/31 08:57:01  cjm
# Library now repackaged as libdprt_ccs. Directory changed to ccs specific one.
#
# Revision 0.11  2004/01/30 16:45:05  cjm
# Added library directory directive.
#
# Revision 0.10  2002/05/20 09:47:02  cjm
# CHanged location on binaries/object files.
#
# Revision 0.9  2001/05/15 19:20:24  cjm
# Added docs make.
#
# Revision 0.8  2001/05/15 18:38:49  cjm
# Split dprt into libdprt.
#
# Revision 0.7  1999/12/10 12:24:43  cjm
# Added backup target.
#
# Revision 0.6  1999/09/08 15:26:33  cjm
# Changed dprt_test so that it links against the shared library.
# This is the default library built so this will work with a top-level make.
#
# Revision 0.5  1999/09/08 13:57:49  cjm
# Changed checkin target to accept failure(due to files not being checked out locked).
# Changed checkin target to use CI_OPTIONS.
#
# Revision 0.4  1999/08/31 12:47:19  cjm
# Replaced LIBDIR with LT_LIB_HOME
#
# Revision 0.3  1999/08/25 10:03:53  cjm
# Updated makefile to take account of ltccd1 directory changes.
#
#

# DO NOT DELETE

/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/stdio.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/features.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/sys/cdefs.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/wordsize.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/gnu/stubs.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/gnu/stubs-32.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/types.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/typesizes.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/libio.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/_G_config.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/wchar.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/wchar.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/gconv.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/stdio_lim.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/sys_errlist.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/stdlib.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/sys/types.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/time.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/endian.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/endian.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/sys/select.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/select.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/sigset.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/time.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/sys/sysmacros.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/bits/pthreadtypes.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/alloca.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/include/strings.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /home/dev/src/libdprt/ccs/include/dprt.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /home/dev/src/libdprt/jni_general/include/dprt_jni_general.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/java/jdk1.6.0_07/include/jni.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /usr/java/jdk1.6.0_07/include/linux/jni_md.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /home/dev/src/libdprt/object/include/object.h
/home/dev/bin/libdprt/ccs/test/i386-linux/dprt_test.o: /home/dev/src/log_udp/include/log_udp.h
