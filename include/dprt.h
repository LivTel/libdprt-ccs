/* dprt.h
** $Header: /space/home/eng/cjm/cvs/libdprt-ccs/include/dprt.h,v 0.1 1999-06-24 11:12:19 dev Exp $
*/
#ifndef DPRT_H
#define DPRT_H

/**
 * TRUE is the value usually returned from routines to indicate success.
 */
#ifndef TRUE
#define TRUE 1
#endif
/**
 * FALSE is the value usually returned from routines to indicate failure.
 */
#ifndef FALSE
#define FALSE 0
#endif

/**
 * This is the length of error string of modules in the library.
 */
#define DPRT_ERROR_STRING_LENGTH	256

/* function declarations */
extern int DpRt_Calibrate_Reduce(char *input_filename,char **output_filename,double *mean_counts,double *peak_counts);
extern int DpRt_Expose_Reduce(char *input_filename,char **output_filename,double *seeing,double *counts,double *x_pix,
		       double *y_pix);
extern void DpRt_Set_Abort(int value);
extern int DpRt_Get_Error_Number(void);
extern void DpRt_Get_Error_String(char *error_string);
#endif
