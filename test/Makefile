# $Header: /space/home/eng/cjm/cvs/libdprt-ccs/test/Makefile,v 0.7 1999-12-10 12:24:43 cjm Exp $

include ../../Makefile.common
include ../Makefile.common

INCDIR = $(DPRT_SRC_HOME)/include

CFLAGS = -g -I$(INCDIR) -c

SRCS = dprt_test.c
OBJS		=	$(SRCS:%.c=%.o)

top: $(DPRT_BIN_HOME)/test/dprt_test

$(DPRT_BIN_HOME)/test/dprt_test: dprt_test.o $(LT_LIB_HOME)/libdprt.so
	cc -o $@ dprt_test.o -ldprt $(TIMELIB) -lm -lc

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	-$(RM) $(RM_OPTIONS) $(DPRT_BIN_HOME)/test/dprt_test $(OBJS) $(TIDY_OPTIONS)

tidy:
	-$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)

backup: tidy
	-$(RM) $(RM_OPTIONS) $(DPRT_BIN_HOME)/test/dprt_test

checkin:
	-$(CI) $(CI_OPTIONS) $(SRCS)

checkout:
	$(CO) $(CO_OPTIONS) $(SRCS)

#
# $Log: not supported by cvs2svn $
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

dprt_test.o: /usr/include/stdio.h /usr/include/sys/feature_tests.h
dprt_test.o: /usr/include/sys/isa_defs.h /usr/include/sys/va_list.h
dprt_test.o: /usr/include/stdio_tag.h /usr/include/stdio_impl.h
dprt_test.o: /usr/include/stdlib.h /usr/include/strings.h
dprt_test.o: /usr/include/sys/types.h /usr/include/sys/machtypes.h
dprt_test.o: /usr/include/sys/int_types.h /usr/include/sys/select.h
dprt_test.o: /usr/include/sys/time.h /usr/include/sys/time.h
dprt_test.o: /usr/include/string.h /space/home/dev/src/dprt/include/dprt.h
