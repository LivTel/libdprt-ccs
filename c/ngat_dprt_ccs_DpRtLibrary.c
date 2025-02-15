/*   
    Copyright 2006, Astrophysics Research Institute, Liverpool John Moores University.

    This file is part of DpRt.

    DpRt is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    DpRt is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DpRt; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
/* ngat_dprt_ccs_DpRtLibrary.c
** Implementation of Java Classes ngat.dprt.ccs.DpRtLibrary.java native Interfaces
** $Header: /space/home/eng/cjm/cvs/libdprt-ccs/c/ngat_dprt_ccs_DpRtLibrary.c,v 1.3 2006-05-16 18:30:40 cjm Exp $
*/
/**
 * ngat_dprt_ccs_DpRtLibrary.c is the  `glue' between libdprt_ccs and ngat.dprt.ccs.DpRtLibrary.java.
 * @author Chris Mottram LJMU
 * @version $Revision: 1.3 $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include "ngat_dprt_ccs_DpRtLibrary.h"
#include "object.h"
#include "dprt.h"
#include "dprt_jni_general.h"

/* -------------------------------------------------- */
/* internal variables */
/* -------------------------------------------------- */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: ngat_dprt_ccs_DpRtLibrary.c,v 1.3 2006-05-16 18:30:40 cjm Exp $";

/* -------------------------------------------------- */
/* internal functions */
/* -------------------------------------------------- */


/* -------------------------------------------------- */
/* external functions */
/* -------------------------------------------------- */
/**
 * This routine gets called when the native library is loaded. We use this routine
 * to get a copy of the JavaVM pointer of the JVM we are running in. This is used to
 * get the correct per-thread JNIEnv context pointer when C calls back into Java.
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Java_VM
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	DpRt_JNI_Set_Java_VM(vm);
	return JNI_VERSION_1_2;
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Initialise<br>
 * Signature: ()V<br>
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @see dprt.html#DpRt_Initialise
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Throw_Exception
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Property_Function_Pointer
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Property_Integer_Function_Pointer
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Property_Double_Function_Pointer
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Property_Boolean_Function_Pointer
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_DpRtStatus_Get_Property
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Property_Integer
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Property_Double
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Property_Boolean
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Log_Handler
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Initialise(JNIEnv *env,jobject object)
{
	int retval;

	/* Overwrite Function Pointers with JNI versions. 
	** We must do this before calling DpRt_Initialise, as this now checks
	** the fake flag. */
	DpRt_JNI_Set_Property_Function_Pointer(DpRt_JNI_DpRtStatus_Get_Property);
	DpRt_JNI_Set_Property_Integer_Function_Pointer(DpRt_JNI_DpRtStatus_Get_Property_Integer);
	DpRt_JNI_Set_Property_Double_Function_Pointer(DpRt_JNI_DpRtStatus_Get_Property_Double);
	DpRt_JNI_Set_Property_Boolean_Function_Pointer(DpRt_JNI_DpRtStatus_Get_Property_Boolean);
	/* set object code logging */
	Object_Set_Log_Handler_Function(DpRt_JNI_Log_Handler);
	/* call c initialisation */
	retval = DpRt_Initialise();
	if(retval != TRUE)
		DpRt_JNI_Throw_Exception(env,"DpRt_Initialise");
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Shutdown<br>
 * Signature: ()V<br>
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * Java Native Interface implementation ngat.dprt.ccs.DpRtLibrary's shutdown.
 * @see dprt.html#DpRt_Shutdown
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Throw_Exception
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Shutdown(JNIEnv *env,jobject object)
{
	int retval;

	retval = DpRt_Shutdown();
	if(retval != TRUE)
		DpRt_JNI_Throw_Exception(env,"DpRt_Shutdown");
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Set_Status<br>
 * Signature: (Lngat/dprt/DpRtStatus;)V<br>
 * Java Native Interface implementation ngat.dprt.ccs.DpRtLibrary's setStatus.
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Status
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Set_1Status(JNIEnv *env,jobject object,jobject status)
{
	DpRt_JNI_Set_Status(env,object,status);
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    initialiseLoggerReference<br>
 * Signature: (Lngat/util/logging/Logger;)V<br>
 * Java Native Interface implementation ngat.dprt.ccs.DpRtLibrary's initialiseLoggerReference.
 * This takes the supplied logger object reference and stores it in the logger variable as a global reference.
 * The log method ID is also retrieved and stored.
 * @param l The DpRtLibrary's "ngat.dprt.ccs.DpRtLibrary" logger.
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Initialise_Logger_Reference
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_initialiseLoggerReference(JNIEnv *env,jobject obj,jobject l)
{
	DpRt_JNI_Initialise_Logger_Reference(env,obj,l);
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    finaliseLoggerReference<br>
 * Signature: ()V<br>
 * This native method is called from ngat.dprt.ccs.DpRtLibrary's finaliser method. It removes the global reference to
 * logger.
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Finalise_Logger_Reference
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_finaliseLoggerReference(JNIEnv *env, jobject obj)
{
	DpRt_JNI_Finalise_Logger_Reference(env);
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Calibrate_Reduce<br>
 * Signature: (Ljava/lang/String;Lngat/message/INST_DP/CALIBRATE_REDUCE_DONE;)Z<br>
 * JNI interface routine called when ngat.dprt.ccs.DpRtLibrary.DpRtCalibrateReduce is called.
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @param input_filename_string The Java String object representing the filename string to be processed.
 * @param reduce_done A Java object of class CALIBRATE_REDUCE_DONE. As a result of the data pipeline the fields of this
 * instance of the class should be filled in.
 * @see dprt.html#DpRt_Calibrate_Reduce
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_Number
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_String
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Command_Done
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Reduce_Done
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Calibrate_Reduce_Done
 */
JNIEXPORT jboolean JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Calibrate_1Reduce(JNIEnv *env, jobject object, 
				 jstring input_filename_string, jobject reduce_done)
{
	char error_string[DPRT_ERROR_STRING_LENGTH];
	const char *input_filename = NULL;
	char *output_filename = NULL;
	double meanCounts = 0.0,peakCounts= 0.0;
	int successful = FALSE;
	int error_number = 0;
	jclass cls;

	/* Get the filename froma java string to a c null terminated string
	** If the java String is null the input_filename should be null as well */
	if(input_filename_string != NULL)
		input_filename = (*env)->GetStringUTFChars(env,input_filename_string,0);

	/* call the reduction process */
	successful = DpRt_Calibrate_Reduce((char*)input_filename,&output_filename,&meanCounts,&peakCounts);

	/* get the error information associated with this call */
	error_number = DpRt_JNI_Get_Error_Number();
	DpRt_JNI_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(input_filename_string != NULL)
		(*env)->ReleaseStringUTFChars(env,input_filename_string,input_filename);

	/* set the relevant fields in reduce_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,reduce_done);

	if(DpRt_JNI_Set_Command_Done(env,cls,reduce_done,successful,error_number,error_string) == FALSE)
		return FALSE;

	if(DpRt_JNI_Set_Reduce_Done(env,cls,reduce_done,output_filename) == FALSE)
		return FALSE;

	/* free output_filename allocated in Reduction */
	if(output_filename != NULL)
		free(output_filename);

	if(DpRt_JNI_Set_Calibrate_Reduce_Done(env,cls,reduce_done,meanCounts,peakCounts) == FALSE)
		return FALSE;

	return TRUE;
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Expose_Reduce<br>
 * Signature: (Ljava/lang/String;Lngat/message/INST_DP/EXPOSE_REDUCE_DONE;)Z<br>
 * JNI interface routine called when ngat.dprt.ccs.DpRtLibrary.DpRtExposeReduce is called.
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @param input_filename_string The Java String object representing the filename string to be processed.
 * @param reduce_done A Java object of class EXPOSE_REDUCE_DONE. As a result of the data pipeline the fields of this
 * 	instance of the class should be filled in.
 * @see dprt.html#DpRt_Expose_Reduce
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_Number
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_String
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Command_Done
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Reduce_Done
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Expose_Reduce_Done
 */
JNIEXPORT jboolean JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Expose_1Reduce(JNIEnv *env,jobject object,
								 jstring input_filename_string,jobject reduce_done)
{
	char error_string[DPRT_ERROR_STRING_LENGTH];
	const char *input_filename = NULL;
	char *output_filename = NULL;
	double seeing = 0.0,counts = 0.0,x_pix = 0.0,y_pix = 0.0;
	double photometricity = 0.0, sky_brightness = 0.0;
	int saturated = FALSE;
	int successful = FALSE;
	int error_number = 0;
	jclass cls;

	/* Get the filename froma java string to a c null terminated string
	** If the java String is null the input_filename should be null as well */
	if(input_filename_string != NULL)
		input_filename = (*env)->GetStringUTFChars(env,input_filename_string,0);

	/* call the reduction process */
	successful = DpRt_Expose_Reduce((char*)input_filename,&output_filename,&seeing,&counts,&x_pix,&y_pix,
					&photometricity,&sky_brightness,&saturated);

	/* get the error information associated with this call */
	error_number = DpRt_JNI_Get_Error_Number();
	DpRt_JNI_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(input_filename_string != NULL)
		(*env)->ReleaseStringUTFChars(env,input_filename_string,input_filename);

	/* set the relevant fields in reduce_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,reduce_done);

	if(DpRt_JNI_Set_Command_Done(env,cls,reduce_done,successful,error_number,error_string) == FALSE)
	{
		/* free output_filename allocated in DpRt_Expose_Reduce */
		if(output_filename != NULL)
			free(output_filename);
		return FALSE;
	}

	if(DpRt_JNI_Set_Reduce_Done(env,cls,reduce_done,output_filename) == FALSE)
	{
		/* free output_filename allocated in DpRt_Expose_Reduce */
		if(output_filename != NULL)
			free(output_filename);
		return FALSE;
	}

	/* free output_filename allocated in Reduction */
	if(output_filename != NULL)
		free(output_filename);

	if(DpRt_JNI_Set_Expose_Reduce_Done(env,cls,reduce_done,seeing,counts,x_pix,y_pix,
					   photometricity,sky_brightness,saturated) == FALSE)
		return FALSE;

	return TRUE;
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Make_Master_Bias<br>
 * Signature: (Ljava/lang/String;Lngat/message/INST_DP/MAKE_MASTER_BIAS_DONE;)Z<br>
 * JNI interface routine called to create a master bias frame.
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @param dirname_jstring The Java String object representing the directory to be processed.
 * @param make_master_bias_done A Java object of class MAKE_MASTER_BIAS_DONE. 
 * 	As a result of the data pipeline the fields of this instance of the class should be filled in.
 * @see dprt.html#DpRt_Make_Master_Bias
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_Number
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_String
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Command_Done
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Reduce_Done
 */
JNIEXPORT jboolean JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Make_1Master_1Bias(JNIEnv *env, jobject object, 
			   jstring dirname_jstring, jobject make_master_bias_done)
{
	char error_string[DPRT_ERROR_STRING_LENGTH];
	const char *dirname_cstring = NULL;
	int successful = FALSE;
	int error_number = 0;
	jclass cls;

	/* Get the filename froma java string to a c null terminated string
	** If the java String is null the dirname_cstring should be null as well */
	if(dirname_jstring != NULL)
		dirname_cstring = (*env)->GetStringUTFChars(env,dirname_jstring,0);

	/* call the reduction process */
	successful = DpRt_Make_Master_Bias((char*)dirname_cstring);

	/* get the error information associated with this call */
	error_number = DpRt_JNI_Get_Error_Number();
	DpRt_JNI_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(dirname_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,dirname_jstring,dirname_cstring);

	/* set the relevant fields in make_master_bias_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,make_master_bias_done);

	if(DpRt_JNI_Set_Command_Done(env,cls,make_master_bias_done,successful,error_number,error_string) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}
/*
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Make_Master_Flat<br>
 * Signature: (Ljava/lang/String;Lngat/message/INST_DP/MAKE_MASTER_FLAT_DONE;)Z<br>
 * JNI interface routine called to create a master flat frame.
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @param dirname_jstring The Java String object representing the directory to be processed.
 * @param make_master_flat_done A Java object of class MAKE_MASTER_FLAT_DONE. 
 * 	As a result of the data pipeline the fields of this instance of the class should be filled in.
 * @see dprt.html#DpRt_Make_Master_Flat
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_Number
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Get_Error_String
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Command_Done
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Reduce_Done
 */
JNIEXPORT jboolean JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Make_1Master_1Flat(JNIEnv *env, jobject object, 
			     jstring dirname_jstring, jobject make_master_flat_done)
{
	char error_string[DPRT_ERROR_STRING_LENGTH];
	const char *dirname_cstring = NULL;
	int successful = FALSE;
	int error_number = 0;
	jclass cls;

	/* Get the filename froma java string to a c null terminated string
	** If the java String is null the dirname_cstring should be null as well */
	if(dirname_jstring != NULL)
		dirname_cstring = (*env)->GetStringUTFChars(env,dirname_jstring,0);

	/* call the reduction process */
	successful = DpRt_Make_Master_Flat((char*)dirname_cstring);

	/* get the error information associated with this call */
	error_number = DpRt_JNI_Get_Error_Number();
	DpRt_JNI_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(dirname_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,dirname_jstring,dirname_cstring);

	/* set the relevant fields in make_master_flat_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,make_master_flat_done);

	if(DpRt_JNI_Set_Command_Done(env,cls,make_master_flat_done,successful,error_number,error_string) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Abort<br>
 * Signature: ()V<br>
 * JNI interface routine called when ngat.dprt.ccs.DpRtLibrary.DpRtAbort is called.
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Set_Abort
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Abort(JNIEnv *env, jobject object)
{
	DpRt_JNI_Set_Abort(TRUE);
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Finalise_References<br>
 * Signature: ()V<br>
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @see ../../jni_general/cdocs/dprt_jni_general.html#DpRt_JNI_Finalise_Status_Reference
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Finalise_1References(JNIEnv *env,jobject object)
{
	DpRt_JNI_Finalise_Status_Reference(env);
}

/* -------------------------------------------------- */
/* internal routines */
/* -------------------------------------------------- */
/*
** $Log: not supported by cvs2svn $
** Revision 1.2  2004/03/31 16:44:25  cjm
** Removed general JNI code and put into dprt_jni_general.c.
**
** Revision 1.1  2004/03/31 08:55:58  cjm
** Initial revision
**
** Revision 0.7  2004/01/30 16:38:02  cjm
** Added logging methods.
**
** Revision 0.6  2004/01/15 16:04:13  cjm
** No change.
**
** Revision 0.5  2002/11/26 17:38:13  cjm
** Integrated librjs_dprt.
**
** Revision 0.4  2002/05/23 10:45:15  cjm
** Fixed setSaturation bug, removed '('.
**
** Revision 0.3  2002/05/20 11:35:31  cjm
** Native method for DpRt_Expose_Reduce sets extra EXPOSE_DONE parameters.
**
** Revision 0.2  2002/05/20 10:46:06  cjm
** Added property file query routines, via Java.
**
** Revision 0.1  1999/06/24 11:06:42  dev
** initial revision
**
*/
