# $Header: /space/home/eng/cjm/cvs/libdprt-ccs/test/Makefile,v 0.1 1999-06-24 11:09:08 dev Exp $

include ../../Makefile.common

LIBDIR = ../c
INCDIR = ../include

CFLAGS = -g -I$(INCDIR) -c

SRCS = dprt_test.c
OBJS		=	$(SRCS:%.c=%.o)

top: dprt_test

dprt_test: dprt_test.o $(LIBDIR)/libdprt.a 
	cc -o dprt_test dprt_test.o $(LIBDIR)/libdprt.a $(TIMELIB) -lm -lc

depend:
	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	$(RM) $(RM_OPTIONS) $(OBJS) $(TIDY_OPTIONS)

tidy:
	$(RM) $(RM_OPTIONS) $(TIDY_OPTIONS)

checkin:
	$(CI) $(SRCS)

checkout:
	$(CO) $(CO_OPTIONS) $(SRCS)

# DO NOT DELETE
