/* dprt.h
** $Header: /space/home/eng/cjm/cvs/libdprt-ccs/include/dprt.h,v 0.3 2002-05-20 11:01:40 cjm Exp $
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
extern void DpRt_Initialise(void);
extern int DpRt_Calibrate_Reduce(char *input_filename,char **output_filename,double *mean_counts,double *peak_counts);
extern int DpRt_Expose_Reduce(char *input_filename,char **output_filename,double *seeing,double *counts,double *x_pix,
		       double *y_pix,double *photometricity,double *sky_brightness,int *saturated);
extern void DpRt_Set_Abort(int value);
extern int DpRt_Get_Property(char *keyword,char **value_string);
extern int DpRt_Get_Property_Integer(char *keyword,int *value);
extern int DpRt_Get_Property_Double(char *keyword,double *value);
extern int DpRt_Get_Property_Boolean(char *keyword,int *value);
extern void DpRt_Set_Property_Function_Pointer(int (*get_property_fp)(char *keyword,char **value_string));
extern void DpRt_Set_Property_Integer_Function_Pointer(int (*get_property_integer_fp)(char *keyword,int *value));
extern void DpRt_Set_Property_Double_Function_Pointer(int (*get_property_double_fp)(char *keyword,double *value));
extern void DpRt_Set_Property_Boolean_Function_Pointer(int (*get_property_boolean_fp)(char *keyword,int *value));
extern int DpRt_Get_Error_Number(void);
extern void DpRt_Get_Error_String(char *error_string);
#endif
