/* dprt_test.c  -*- mode: Fundamental;-*-
** dprt_test [-c][-e][-help] <filename>
** $Header: /space/home/eng/cjm/cvs/libdprt-ccs/test/dprt_test.c,v 0.2 2002-05-20 09:42:30 cjm Exp $
*/
/**
 * dprt_test.c Tests libdprt.a, the statically linked version of the Data Pipeline Real Time
 * reduction library. Note you cannot check Aborting reductions with this software at the moment.
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "dprt.h"

/* ------------------------------------------------------- */
/* internal hash definitions */
/* ------------------------------------------------------- */
/**
 * Reduce Type definition. This means the file should be reduced as an exposure.
 */
#define REDUCE_TYPE_EXPOSE		1
/**
 * Reduce Type definition. This means the file should be reduced as an calibration.
 */
#define REDUCE_TYPE_CALIBRATION		2

/* ------------------------------------------------------- */
/* internal functions declarations */
/* ------------------------------------------------------- */
static void Help(void);
static int Parse_Args(int argc,char *argv[]);

/* ------------------------------------------------------- */
/* internal variables */
/* ------------------------------------------------------- */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: dprt_test.c,v 0.2 2002-05-20 09:42:30 cjm Exp $";
/**
 * Filename of file to be processed.
 */
static char Filename[256] = "";
/**
 * The type of reduction to perform on the file.
 */
static int Reduce_Type = REDUCE_TYPE_EXPOSE;

/* ------------------------------------------------------- */
/* external functions */
/* ------------------------------------------------------- */
/**
 * The main program.
 */
int main(int argc, char *argv[])
{
	char error_string[DPRT_ERROR_STRING_LENGTH];
	char *output_filename = NULL;/* output filename of reduction */
	double seeing = 0.0;/* seeing returned by reduction */
	double counts = 0.0;/* returned by reduction */
	double mean_counts = 0.0;/* returned by reduction */
	double peak_counts = 0.0;/* returned by reduction */
	double x_pix = 0.0;/* returned by reduction */
	double y_pix = 0.0;/* returned by reduction */

	if(argc < 2)
	{
		Help();
		return 0;
	}
	if(!Parse_Args(argc,argv))
		return 0;
	if(strcmp(Filename,"")==0)
	{
		fprintf(stderr,"dprt_test: No filename specified.\n");
		return 1;
	}
/* initialise the DpRt */
	DpRt_Initialise();
	if(Reduce_Type == REDUCE_TYPE_EXPOSE)
	{
		fprintf(stdout,"Reducing file '%s' as an exposure.\n",Filename);
		if(DpRt_Expose_Reduce(Filename,&output_filename,&seeing,&counts,&x_pix,&y_pix))
		{
			fprintf(stdout,"Reduction returned:output_filename:%s,seeing:%.2f,counts:%.2f,"
				"x_pix:%.2f,y_pix:%.2f\n",output_filename,seeing,counts,x_pix,y_pix);
		}
		else
		{
			DpRt_Get_Error_String(error_string);
			fprintf(stderr,"DpRt_Expose_Reduce failed:(%d) %s.\n",DpRt_Get_Error_Number(),error_string);
		}
		
	}
	else if(Reduce_Type == REDUCE_TYPE_CALIBRATION)
	{
		fprintf(stdout,"Reducing file '%s' as an calibration.\n",Filename);
		if(DpRt_Calibrate_Reduce(Filename,&output_filename,&mean_counts,&peak_counts))
		{
			fprintf(stdout,"Reduction returned:output_filename:%s,mean counts:%.2f,peak counts:%.2f\n",
				output_filename,mean_counts,peak_counts);
		}
		else
		{
			DpRt_Get_Error_String(error_string);
			fprintf(stderr,"DpRt_Calibrate_Reduce failed:(%d) %s.\n",DpRt_Get_Error_Number(),error_string);
		}
	}
	else
	{
		fprintf(stderr,"dprt_test: Unknown reduction type %d specified.\n",Reduce_Type);
		return 1;
	}
	if(output_filename != NULL)
		free(output_filename);
	output_filename = NULL;
	fprintf(stdout,"Reduction completed.\n");
	return 0;
}

/* ------------------------------------------------------- */
/* internal functionss */
/* ------------------------------------------------------- */
/**
 * Routine to parse arguments.
 * @param argc The argument count.
 * @param argv The argument list.
 * @return Returns TRUE if the program can proceed, FALSE if it should stop (the user requested help).
 */
static int Parse_Args(int argc,char *argv[])
{
	int i;
	int call_help = FALSE;

	strcpy(Filename,"");
	for(i=1;i<argc;i++)
	{
		if(strcmp(argv[i],"-help")==0)
			call_help = TRUE;
		else if(strcmp(argv[i],"-c")==0)
			Reduce_Type = REDUCE_TYPE_CALIBRATION;
		else if(strcmp(argv[i],"-e")==0)
			Reduce_Type = REDUCE_TYPE_EXPOSE;
		else
			strcpy(Filename,argv[i]);
	}
	if(call_help)
	{
		Help();
		return FALSE;
	}
	return TRUE;
}

/**
 * Routine to produce some help.
 */
static void Help(void)
{
	fprintf(stdout,"dprt_test Tests the reduction routines in libdprt.\n");
	fprintf(stdout,"dprt_test does NOT test the Java JNI interface or aborting reductions.\n");
	fprintf(stdout,"dprt_test [-c] [-e] [-help] <filename>\n");
	fprintf(stdout,"-c reduces the filename as a calibration image.\n");
	fprintf(stdout,"-e reduces the filename as a expose image.\n");
	fprintf(stdout,"-help prints this help message and exits.\n");
	fprintf(stdout,"You must always specify a filename to reduce.\n");
}
/*
** $Log: not supported by cvs2svn $
** Revision 0.1  1999/06/24 11:10:24  dev
** initial revision
**
*/






