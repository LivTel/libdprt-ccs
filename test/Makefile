# $Header: /space/home/eng/cjm/cvs/libdprt-ccs/test/Makefile,v 0.4 1999-08-31 12:47:19 cjm Exp $

include ../../Makefile.common
include ../Makefile.common

INCDIR = $(DPRT_SRC_HOME)/include

CFLAGS = -g -I$(INCDIR) -c

SRCS = dprt_test.c
OBJS		=	$(SRCS:%.c=%.o)

top: $(DPRT_BIN_HOME)/test/dprt_test

$(DPRT_BIN_HOME)/test/dprt_test: dprt_test.o $(LT_LIB_HOME)/libdprt.a 
	cc -o $@ dprt_test.o $(LT_LIB_HOME)/libdprt.a $(TIMELIB) -lm -lc

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	$(RM) $(RM_OPTIONS) $(DPRT_BIN_HOME)/test/dprt_test $(OBJS) $(TIDY_OPTIONS)

tidy:
	$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)

checkin:
	$(CI) $(SRCS)

checkout:
	$(CO) $(CO_OPTIONS) $(SRCS)

#
# $Log: not supported by cvs2svn $
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
