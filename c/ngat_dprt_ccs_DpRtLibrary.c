/* ngat_dprt_ccs_DpRtLibrary.c
** Implementation of Java Classes ngat.dprt.ccs.DpRtLibrary.java native Interfaces
** $Header: /space/home/eng/cjm/cvs/libdprt-ccs/c/ngat_dprt_ccs_DpRtLibrary.c,v 1.1 2004-03-31 08:55:58 cjm Exp $
*/
/**
 * ngat_dprt_ccs_DpRtLibrary.c is the  `glue' between libdprt_ccs and ngat.dprt.ccs.DpRtLibrary.java.
 * @author Chris Mottram LJMU
 * @version $Revision: 1.1 $
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include "ngat_dprt_ccs_DpRtLibrary.h"
#include "object.h"
#include "dprt.h"

/* -------------------------------------------------- */
/* internal variables */
/* -------------------------------------------------- */
/**
 * Revision Control System identifier.
 */
static char rcsid[] = "$Id: ngat_dprt_ccs_DpRtLibrary.c,v 1.1 2004-03-31 08:55:58 cjm Exp $";
/**
 * Internal Error Number - set this to a unique value for each location an internal error occurs.
 */
static int DpRtLibrary_Error_Number = 0;
/**
 * Internal Error String - set this to a descriptive string each place an error occurs.
 * Ensure the string is not longer than <a href="#DPRT_ERROR_STRING_LENGTH">DPRT_ERROR_STRING_LENGTH</a> long.
 * @see #DpRtLibrary_Error_Number
 * @see #DPRT_ERROR_STRING_LENGTH
 */
static char DpRtLibrary_Error_String[DPRT_ERROR_STRING_LENGTH] = "";
/**
 * Copy of the java virtual machine pointer, used for calling back up to the Java layer from C.
 */
static JavaVM *Java_VM = NULL;
/**
 * Cached global reference to the "DpRtLibrary" logger, used to log back to the Java layer from
 * C routines.
 */
static jobject Logger = NULL;
/**
 * Cached reference to the "ngat.util.logging.Logger" class's log(int level,String message) method.
 * Used to log C layer log messages, in conjunction with the logger's object reference logger.
 * @see #logger
 */
static jmethodID Log_Method_Id = NULL;
/**
 * Cached global reference to the "ngat.dprt.DpRtStatus" instance, so that C routines can access Java methods.
 */
static jobject DpRt_Status = NULL;
/**
 * Cached reference to the "ngat.dprt.DpRtStatus" class's getProperty(String keyword) method.
 * Used to retrieve configuration from the Java property file from the C layer.
 * @see #DpRt_Status
 */
static jmethodID DpRt_Status_Get_Property_Method_Id = NULL;
/**
 * Cached reference to the "ngat.dprt.DpRtStatus" class's getPropertyInteger(String keyword) method.
 * Used to retrieve configuration from the Java property file from the C layer.
 * @see #DpRt_Status
 */
static jmethodID DpRt_Status_Get_Property_Integer_Method_Id = NULL;
/**
 * Cached reference to the "ngat.dprt.DpRtStatus" class's getPropertyDouble(String keyword) method.
 * Used to retrieve configuration from the Java property file from the C layer.
 * @see #DpRt_Status
 */
static jmethodID DpRt_Status_Get_Property_Double_Method_Id = NULL;
/**
 * Cached reference to the "ngat.dprt.DpRtStatus" class's getPropertyBoolean(String keyword) method.
 * Used to retrieve configuration from the Java property file from the C layer.
 * @see #DpRt_Status
 */
static jmethodID DpRt_Status_Get_Property_Boolean_Method_Id = NULL;

/* -------------------------------------------------- */
/* internal functions */
/* -------------------------------------------------- */
static int DpRtLibrary_Set_Command_Done(JNIEnv *env,jclass cls,jobject done,
					int successful,int error_number,char *error_string);
static int DpRtLibrary_Set_Reduce_Done(JNIEnv *env,jclass cls,jobject done,char *output_filename);
static void DpRtLibrary_Throw_Exception(JNIEnv *env,char *function_name);
static void DpRtLibrary_Throw_Exception_String(JNIEnv *env,char *function_name,int error_number,char *error_string);
static int DpRtLibrary_Get_Property(char *keyword,char **value_string);
static int DpRtLibrary_Get_Property_Integer(char *keyword,int *value);
static int DpRtLibrary_Get_Property_Double(char *keyword,double *value);
static int DpRtLibrary_Get_Property_Boolean(char *keyword,int *value);
static void DpRtLibrary_Log_Handler(int level,char *string);

/* -------------------------------------------------- */
/* external functions */
/* -------------------------------------------------- */
/**
 * This routine gets called when the native library is loaded. We use this routine
 * to get a copy of the JavaVM pointer of the JVM we are running in. This is used to
 * get the correct per-thread JNIEnv context pointer when C calls back into Java.
 * @see #Java_VM
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	Java_VM = vm;
	return JNI_VERSION_1_2;
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Initialise<br>
 * Signature: ()V<br>
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @see #DpRtLibrary_Throw_Exception
 * @see dprt.html#DpRt_Initialise
 * @see dprt.html#DpRt_Set_Property_Function_Pointer
 * @see dprt.html#DpRt_Set_Property_Integer_Function_Pointer
 * @see dprt.html#DpRt_Set_Property_Double_Function_Pointer
 * @see dprt.html#DpRt_Set_Property_Boolean_Function_Pointer
 * @see #DpRtLibrary_Get_Property
 * @see #DpRtLibrary_Get_Property_Integer
 * @see #DpRtLibrary_Get_Property_Double
 * @see #DpRtLibrary_Get_Property_Boolean
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Initialise(JNIEnv *env,jobject object)
{
	int retval;

	/* Overwrite Function Pointers with JNI versions. 
	** We must do this before calling DpRt_Initialise, as this now checks
	** the fake flag. */
	DpRt_Set_Property_Function_Pointer(DpRtLibrary_Get_Property);
	DpRt_Set_Property_Integer_Function_Pointer(DpRtLibrary_Get_Property_Integer);
	DpRt_Set_Property_Double_Function_Pointer(DpRtLibrary_Get_Property_Double);
	DpRt_Set_Property_Boolean_Function_Pointer(DpRtLibrary_Get_Property_Boolean);
	/* set object code logging */
	Object_Set_Log_Handler_Function(DpRtLibrary_Log_Handler);
	/* call c initialisation */
	retval = DpRt_Initialise();
	if(retval != TRUE)
		DpRtLibrary_Throw_Exception(env,"DpRt_Initialise");
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Shutdown<br>
 * Signature: ()V<br>
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @see #DpRtLibrary_Throw_Exception
 * Java Native Interface implementation Dngat.dprt.ccs.pRtLibrary's shutdown.
 * @see dprt.html#DpRt_Shutdown
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Shutdown(JNIEnv *env,jobject object)
{
	int retval;

	retval = DpRt_Shutdown();
	if(retval != TRUE)
		DpRtLibrary_Throw_Exception(env,"DpRt_Shutdown");
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Set_Status<br>
 * Signature: (Lngat/dprt/DpRtStatus;)V<br>
 * Java Native Interface implementation ngat.dprt.ccs.DpRtLibrary's setStatus.
 * This takes the supplied ngat.dprt.DpRtStatus object reference and stores it in the 
 * DpRt_Status variable as a global reference.
 * Some method ID's from this class are also retrieved and stored.
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @param status The DpRt's instance of ngat.dprt.DpRtStatus.
 * @see #DpRt_Status
 * @see #DpRt_Status_Get_Property_Method_Id
 * @see #DpRt_Status_Get_Property_Integer_Method_Id
 * @see #DpRt_Status_Get_Property_Double_Method_Id
 * @see #DpRt_Status_Get_Property_Boolean_Method_Id
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Set_1Status(JNIEnv *env,jobject object,jobject status)
{
	jclass cls = NULL;

/* save DpRtStatus instance */
	DpRt_Status = (*env)->NewGlobalRef(env,status);
/* get the DpRtStatus class */
	cls = (*env)->FindClass(env,"ngat/dprt/DpRtStatus");
	/* if the class is null, one of the following exceptions occured:
	** ClassFormatError,ClassCircularityError,NoClassDefFoundError,OutOfMemoryError */
	if(cls == NULL)
		return;
/* get relevant method id's to call */
/* String getProperty(java/lang/String keyword) */
	DpRt_Status_Get_Property_Method_Id = (*env)->GetMethodID(env,cls,"getProperty",
								 "(Ljava/lang/String;)Ljava/lang/String;");
	if(DpRt_Status_Get_Property_Method_Id == NULL)
	{
		/* One of the following exceptions has been thrown:
		** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
		return;
	}
/* int getPropertyInteger(java/lang/String keyword) */
	DpRt_Status_Get_Property_Integer_Method_Id = (*env)->GetMethodID(env,cls,"getPropertyInteger",
								 "(Ljava/lang/String;)I");
	if(DpRt_Status_Get_Property_Integer_Method_Id == NULL)
	{
		/* One of the following exceptions has been thrown:
		** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
		return;
	}
/* double getPropertyDouble(java/lang/String keyword) */
	DpRt_Status_Get_Property_Double_Method_Id = (*env)->GetMethodID(env,cls,"getPropertyDouble",
								 "(Ljava/lang/String;)D");
	if(DpRt_Status_Get_Property_Double_Method_Id == NULL)
	{
		/* One of the following exceptions has been thrown:
		** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
		return;
	}
/* boolean getPropertyBoolean(java/lang/String keyword) */
	DpRt_Status_Get_Property_Boolean_Method_Id = (*env)->GetMethodID(env,cls,"getPropertyBoolean",
								 "(Ljava/lang/String;)Z");
	if(DpRt_Status_Get_Property_Boolean_Method_Id == NULL)
	{
		/* One of the following exceptions has been thrown:
		** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
		return;
	}
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    initialiseLoggerReference<br>
 * Signature: (Lngat/util/logging/Logger;)V<br>
 * Java Native Interface implementation ngat.dprt.ccs.DpRtLibrary's initialiseLoggerReference.
 * This takes the supplied logger object reference and stores it in the logger variable as a global reference.
 * The log method ID is also retrieved and stored.
 * @param l The DpRtLibrary's "ngat.dprt.ccs.DpRtLibrary" logger.
 * @see #Logger
 * @see #Log_Method_Id
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_initialiseLoggerReference(JNIEnv *env,jobject obj,jobject l)
{
	jclass cls = NULL;

/* save logger instance */
	Logger = (*env)->NewGlobalRef(env,l);
/* get the ngat.util.logging.Logger class */
	cls = (*env)->FindClass(env,"ngat/util/logging/Logger");
	/* if the class is null, one of the following exceptions occured:
	** ClassFormatError,ClassCircularityError,NoClassDefFoundError,OutOfMemoryError */
	if(cls == NULL)
		return;
/* get relevant method id to call */
/* log(int level,java/lang/String message) */
	Log_Method_Id = (*env)->GetMethodID(env,cls,"log","(ILjava/lang/String;)V");
	if(Log_Method_Id == NULL)
	{
		/* One of the following exceptions has been thrown:
		** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
		return;
	}
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    finaliseLoggerReference<br>
 * Signature: ()V<br>
 * This native method is called from ngat.dprt.ccs.DpRtLibrary's finaliser method. It removes the global reference to
 * logger.
 * @see #Logger
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_finaliseLoggerReference(JNIEnv *env, jobject obj)
{
	(*env)->DeleteGlobalRef(env,Logger);
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
 * @see dprt.html#DpRt_Get_Error_Number
 * @see dprt.html#DpRt_Get_Error_String
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
	jmethodID mid;

	/* Get the filename froma java string to a c null terminated string
	** If the java String is null the input_filename should be null as well */
	if(input_filename_string != NULL)
		input_filename = (*env)->GetStringUTFChars(env,input_filename_string,0);

	/* call the reduction process */
	successful = DpRt_Calibrate_Reduce((char*)input_filename,&output_filename,&meanCounts,&peakCounts);

	/* get the error information associated with this call */
	error_number = DpRt_Get_Error_Number();
	DpRt_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(input_filename_string != NULL)
		(*env)->ReleaseStringUTFChars(env,input_filename_string,input_filename);

	/* set the relevant fields in reduce_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,reduce_done);

	if(DpRtLibrary_Set_Command_Done(env,cls,reduce_done,successful,error_number,error_string) == FALSE)
		return FALSE;

	if(DpRtLibrary_Set_Reduce_Done(env,cls,reduce_done,output_filename) == FALSE)
		return FALSE;

	/* free output_filename allocated in Reduction */
	if(output_filename != NULL)
		free(output_filename);

	/* meanCounts */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setMeanCounts","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method	*/
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)meanCounts);

	/* peakCounts */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setPeakCounts","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)peakCounts);

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
 * @see dprt.html#DpRt_Get_Error_Number
 * @see dprt.html#DpRt_Get_Error_String
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
	jmethodID mid;

	/* Get the filename froma java string to a c null terminated string
	** If the java String is null the input_filename should be null as well */
	if(input_filename_string != NULL)
		input_filename = (*env)->GetStringUTFChars(env,input_filename_string,0);

	/* call the reduction process */
	successful = DpRt_Expose_Reduce((char*)input_filename,&output_filename,&seeing,&counts,&x_pix,&y_pix,
					&photometricity,&sky_brightness,&saturated);

	/* get the error information associated with this call */
	error_number = DpRt_Get_Error_Number();
	DpRt_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(input_filename_string != NULL)
		(*env)->ReleaseStringUTFChars(env,input_filename_string,input_filename);

	/* set the relevant fields in reduce_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,reduce_done);

	if(DpRtLibrary_Set_Command_Done(env,cls,reduce_done,successful,error_number,error_string) == FALSE)
	{
		/* free output_filename allocated in DpRt_Expose_Reduce */
		if(output_filename != NULL)
			free(output_filename);
		return FALSE;
	}

	if(DpRtLibrary_Set_Reduce_Done(env,cls,reduce_done,output_filename) == FALSE)
	{
		/* free output_filename allocated in DpRt_Expose_Reduce */
		if(output_filename != NULL)
			free(output_filename);
		return FALSE;
	}

	/* free output_filename allocated in Reduction */
	if(output_filename != NULL)
		free(output_filename);

	/* seeing */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setSeeing","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)seeing);

	/* counts */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setCounts","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)counts);

	/* x_pix */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setXpix","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)x_pix);

	/* y_pix */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setYpix","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)y_pix);

	/* photometricity */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setPhotometricity","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)photometricity);

	/* sky_brightness */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setSkyBrightness","(F)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(float)sky_brightness);

	/* saturated */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setSaturation","(Z)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,reduce_done,mid,(jboolean)saturated);
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
 * @see #DpRtLibrary_Set_Command_Done
 * @see dprt.html#DpRt_Make_Master_Bias
 * @see dprt.html#DpRt_Get_Error_Number
 * @see dprt.html#DpRt_Get_Error_String
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
	error_number = DpRt_Get_Error_Number();
	DpRt_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(dirname_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,dirname_jstring,dirname_cstring);

	/* set the relevant fields in make_master_bias_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,make_master_bias_done);

	if(DpRtLibrary_Set_Command_Done(env,cls,make_master_bias_done,successful,error_number,error_string) == FALSE)
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
 * @see #DpRtLibrary_Set_Command_Done
 * @see dprt.html#DpRt_Make_Master_Flat
 * @see dprt.html#DpRt_Get_Error_Number
 * @see dprt.html#DpRt_Get_Error_String
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
	error_number = DpRt_Get_Error_Number();
	DpRt_Get_Error_String(error_string);

	/* free any c strings allocated */
	if(dirname_jstring != NULL)
		(*env)->ReleaseStringUTFChars(env,dirname_jstring,dirname_cstring);

	/* set the relevant fields in make_master_flat_done */
	/* get the class of the object passed in */
	cls = (*env)->GetObjectClass(env,make_master_flat_done);

	if(DpRtLibrary_Set_Command_Done(env,cls,make_master_flat_done,successful,error_number,error_string) == FALSE)
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
 * @see dprt.html#DpRt_Set_Abort
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Abort(JNIEnv *env, jobject object)
{
	DpRt_Set_Abort(TRUE);
}

/**
 * Class:     ngat_dprt_ccs_DpRtLibrary<br>
 * Method:    DpRt_Finalise_References<br>
 * Signature: ()V<br>
 * @param env The JNI environment pointer.
 * @param object The instance of ngat.dprt.ccs.DpRtLibrary this method was called with.
 * @see #DpRt_Status
 */
JNIEXPORT void JNICALL Java_ngat_dprt_ccs_DpRtLibrary_DpRt_1Finalise_1References(JNIEnv *env,jobject object)
{
	if(DpRt_Status != NULL)
		(*env)->DeleteGlobalRef(env,DpRt_Status);
}

/* -------------------------------------------------- */
/* internal routines */
/* -------------------------------------------------- */
/**
 * Internal routine to set the COMMAND_DONE return parameters for a JNI command.
 * @param env The usual JNI parameter.
 * @param cls The JNI class identifier to get the methods for.
 * @param done The object to call the methods for.
 * @param successful The value to set the COMMAND_DONE.successful to.
 * @param error_number The value to set the COMMAND_DONE.errorNumber to.
 * @param error_string The value to set the COMMAND_DONE.errorString to.
 * @return TRUE if all the methods were called successfully, FALSE if a method call failed.
 */
static int DpRtLibrary_Set_Command_Done(JNIEnv *env,jclass cls,jobject done,
					int successful,int error_number,char *error_string)
{
	jmethodID mid;

	/* successful */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setSuccessful","(Z)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,done,mid,successful);

	/* error number */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setErrorNum","(I)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,done,mid,error_number);

	/* error string */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setErrorString","(Ljava/lang/String;)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	(*env)->CallVoidMethod(env,done,mid,(*env)->NewStringUTF(env,error_string));

	return TRUE;
}


/**
 * Internal routine to set the REDUCE_DONE return parameters for a JNI command.
 * @param env The usual JNI parameter.
 * @param cls The JNI class identifier to get the methods for.
 * @param done The object to call the methods for.
 * @param output_filename The value to set the REDUCE_DONE.filename to.
 * @return TRUE if all the methods were called successfully, FALSE if a method call failed.
 */
static int DpRtLibrary_Set_Reduce_Done(JNIEnv *env,jclass cls,jobject done,char *output_filename)
{
	jmethodID mid;

	/* output_filename */
	/* get the method id in this class */
	mid = (*env)->GetMethodID(env,cls,"setFilename","(Ljava/lang/String;)V");
	/* did we find the method id? */
	if (mid == 0)
		return FALSE;
	/* call the method */
	if(output_filename != NULL)
		(*env)->CallVoidMethod(env,done,mid,(*env)->NewStringUTF(env,output_filename));
	else
		(*env)->CallVoidMethod(env,done,mid,NULL);
	return TRUE;
}

/* ------------------------------------------------------------------------------
** 		Internal routines
** ------------------------------------------------------------------------------ */
/**
 * This routine throws an exception. The error generated is from the error codes in dprt, it assumes
 * another routine has generated an error and this routine packs this error into an exception to return
 * to the Java code, using DpRtLibrary_Throw_Exception_String. The total length of the error string should
 * not be longer than DPRT_ERROR_STRING_LENGTH. A new line is added to the start of the error string,
 * so that the error string returned from libdprt is formatted properly.
 * @param env The JNI environment pointer.
 * @param function_name The name of the function in which this exception is being generated for.
 * @see dprt.html#DpRt_Get_Error_Number
 * @see dprt.html#DpRt_Get_Error_String
 * @see #DpRtLibrary_Throw_Exception_String
 * @see #DPRT_ERROR_STRING_LENGTH
 */
static void DpRtLibrary_Throw_Exception(JNIEnv *env,char *function_name)
{
	char error_string[DPRT_ERROR_STRING_LENGTH];
	int error_number;

	error_number = DpRt_Get_Error_Number();
	strcpy(error_string,"\n");
	DpRt_Get_Error_String(error_string+strlen(error_string));
	DpRtLibrary_Throw_Exception_String(env,function_name,error_number,error_string);
}

/**
 * This routine throws an exception of class ngat.dprt.ccs.DpRtLibraryNativeException. This is used to report
 * all libdprt error messages back to the Java layer.
 * @param env The JNI environment pointer.
 * @param function_name The name of the function in which this exception is being generated for.
 * @param error_number The error number to pass to the constructor of the exception.
 * @param error_string The string to pass to the constructor of the exception.
 * @see #DpRtLibrary_Error_Number
 * @see #DpRtLibrary_Error_String
 */
static void DpRtLibrary_Throw_Exception_String(JNIEnv *env,char *function_name,int error_number,char *error_string)
{
	jclass exception_class = NULL;
	jobject exception_instance = NULL;
	jstring error_jstring = NULL;
	jstring dprt_library_error_jstring = NULL;
	jmethodID mid;
	int retval;

	exception_class = (*env)->FindClass(env,"ngat/dprt/DpRtLibraryNativeException");
	if(exception_class == NULL)
	{
		fprintf(stderr,"DpRtLibrary_Throw_Exception_String:FindClass failed:%s:%d:%s\n",function_name,
			error_number,error_string);
		return;
	}
/* get ngat.dprt.ccs.DpRtLibraryNativeException(int errorNumber,String errorString) constructor */
	mid = (*env)->GetMethodID(env,exception_class,"<init>","(ILjava/lang/String;ILjava/lang/String;)V");
	if(mid == 0)
	{
		/* One of the following exceptions has been thrown:
		** NoSuchMethodError, ExceptionInInitializerError, OutOfMemoryError */
		fprintf(stderr,"DpRtLibrary_Throw_Exception_String:GetMethodID failed:%s:%s\n",function_name,
			error_string);
		return;
	}
/* convert error_string to JString */
	error_jstring = (*env)->NewStringUTF(env,error_string);
	dprt_library_error_jstring = (*env)->NewStringUTF(env,DpRtLibrary_Error_String);
/* call constructor */
	exception_instance = (*env)->NewObject(env,exception_class,mid,(jint)DpRtLibrary_Error_Number,
					       dprt_library_error_jstring,
					       (jint)error_number,error_jstring);
	if(exception_instance == NULL)
	{
		/* One of the following exceptions has been thrown:
		** InstantiationException, OutOfMemoryError */
		fprintf(stderr,"DpRtLibrary_Throw_Exception_String:NewObject failed %s:%d:%s:%d:%s\n",
			function_name,DpRtLibrary_Error_Number,DpRtLibrary_Error_String,error_number,error_string);
		return;
	}
/* throw instance */
	retval = (*env)->Throw(env,(jthrowable)exception_instance);
	if(retval !=0)
	{
		fprintf(stderr,"DpRtLibrary_Throw_Exception_String:Throw failed %d:%s:%d:%s:%d:%s\n",retval,
			function_name,DpRtLibrary_Error_Number,DpRtLibrary_Error_String,error_number,error_string);
	}
}

/**
 * Routine to get the value of the keyword from the properties held in the instance of DpRtStatus.
 * @param keyword The keyword in the property file to look up.
 * @param value_string The address of a pointer to allocate and store the resulting value string in.
 * 	This pointer is dynamically allocated and must be freed using <b>free()</b>. 
 * @see #DpRt_Status
 * @see #DpRt_Status_Get_Property_Method_Id
 */
static int DpRtLibrary_Get_Property(char *keyword,char **value_string)
{
	JNIEnv *env = NULL;
	jstring java_keyword_string = NULL;
	jobject java_value_object = NULL;
	jstring java_value_string = NULL;
	const char *c_value_string = NULL;

	if(DpRt_Status == NULL)
	{
		DpRtLibrary_Error_Number = 1;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property:DpRt_Status was NULL (%s).\n",keyword);
		return FALSE;
	}
	if(DpRt_Status_Get_Property_Method_Id == NULL)
	{
		DpRtLibrary_Error_Number = 2;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property:Method ID was NULL (%s).\n",keyword);
		return FALSE;
	}
	if(Java_VM == NULL)
	{
		DpRtLibrary_Error_Number = 3;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property:Java_VM was NULL (%s).\n",keyword);
		return FALSE;
	}
/* get java env for this thread */
	(*Java_VM)->AttachCurrentThread(Java_VM,(void**)&env,NULL);
	if(env == NULL)
	{
		DpRtLibrary_Error_Number = 4;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property:env was NULL (%s).\n",keyword);
		return FALSE;
	}
	if(keyword == NULL)
	{
		DpRtLibrary_Error_Number = 5;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property:string (%s) was NULL.\n",keyword);
		return FALSE;
	}
	if(value_string == NULL)
	{
		DpRtLibrary_Error_Number = 6;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property:value_string Pointer was NULL.\n");
		return FALSE;
	}
/* convert C to Java String */
	java_keyword_string = (*env)->NewStringUTF(env,keyword);
/* call getProperty method on DpRt_Status instance */
	java_value_object = (*env)->CallObjectMethod(env,DpRt_Status,DpRt_Status_Get_Property_Method_Id,
			java_keyword_string);
/* Convert Java JString to C character array */
	java_value_string = (jstring)java_value_object;
	if(java_value_string != NULL)
		c_value_string = (*env)->GetStringUTFChars(env,java_value_string,0);
/* copy c_value_string to (*value_string) */
	if(c_value_string != NULL)
	{
		(*value_string) = (char *)malloc((strlen(c_value_string)+1)*sizeof(char));
		if((*value_string) == NULL)
		{
			DpRtLibrary_Error_Number = 7;
			sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property:Memory allocation error(%s,%d).\n",
				c_value_string,strlen(c_value_string));
			return FALSE;
		}
		strcpy((*value_string),c_value_string);
	}
	else
		(*value_string) = NULL;
/* free c_value_string */
	if(java_value_string != NULL)
		(*env)->ReleaseStringUTFChars(env,java_value_string,c_value_string);
	return TRUE;
}

/**
 * Routine to get the integer value of the keyword from the properties held in the instance of DpRtStatus.
 * @param keyword The keyword in the property file to look up.
 * @param value The address of an integer to store the resulting integer value in.
 * @see #DpRt_Status
 * @see #DpRt_Status_Get_Property_Integer_Method_Id
 */
static int DpRtLibrary_Get_Property_Integer(char *keyword,int *value)
{
	JNIEnv *env = NULL;
	jstring java_keyword_string = NULL;

	if(DpRt_Status == NULL)
	{
		DpRtLibrary_Error_Number = 8;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Integer:DpRt_Status was NULL (%s).\n",
			keyword);
		return FALSE;
	}
	if(DpRt_Status_Get_Property_Integer_Method_Id == NULL)
	{
		DpRtLibrary_Error_Number = 9;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Integer:Method ID was NULL(%s).\n",keyword);
		return FALSE;
	}
	if(Java_VM == NULL)
	{
		DpRtLibrary_Error_Number = 10;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Integer:Java_VM was NULL(%s).\n",keyword);
		return FALSE;
	}
/* get java env for this thread */
	(*Java_VM)->AttachCurrentThread(Java_VM,(void**)&env,NULL);
	if(env == NULL)
	{
		DpRtLibrary_Error_Number = 11;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Integer:env was NULL (%s).\n",keyword);
		return FALSE;
	}
	if(keyword == NULL)
	{
		DpRtLibrary_Error_Number = 12;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Integer:string (%s) was NULL.\n",keyword);
		return FALSE;
	}
	if(value == NULL)
	{
		DpRtLibrary_Error_Number = 13;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Integer:value Pointer was NULL.\n");
		return FALSE;
	}
/* convert C to Java String */
	java_keyword_string = (*env)->NewStringUTF(env,keyword);
/* call getProperty method on DpRt_Status instance */
	(*value) = (int)((*env)->CallIntMethod(env,DpRt_Status,DpRt_Status_Get_Property_Integer_Method_Id,
			java_keyword_string));
	return TRUE;
}

/**
 * Routine to get the double value of the keyword from the properties held in the instance of DpRtStatus.
 * @param keyword The keyword in the property file to look up.
 * @param value The address of an double to store the resulting value in.
 * @see #DpRt_Status
 * @see #DpRt_Status_Get_Property_Double_Method_Id
 */
static int DpRtLibrary_Get_Property_Double(char *keyword,double *value)
{
	JNIEnv *env = NULL;
	jstring java_keyword_string = NULL;

	if(DpRt_Status == NULL)
	{
		DpRtLibrary_Error_Number = 14;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Double:DpRt_Status was NULL(%s).\n",
			keyword);
		return FALSE;
	}
	if(DpRt_Status_Get_Property_Double_Method_Id == NULL)
	{
		DpRtLibrary_Error_Number = 15;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Double:Method ID was NULL (%s).\n",keyword);
		return FALSE;
	}
	if(Java_VM == NULL)
	{
		DpRtLibrary_Error_Number = 16;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Double:Java_VM was NULL(%s).\n",keyword);
		return FALSE;
	}
/* get java env for this thread */
	(*Java_VM)->AttachCurrentThread(Java_VM,(void**)&env,NULL);
	if(env == NULL)
	{
		DpRtLibrary_Error_Number = 17;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Double:env was NULL (%s).\n",keyword);
		return FALSE;
	}
	if(keyword == NULL)
	{
		DpRtLibrary_Error_Number = 18;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Double:string (%s) was NULL.\n",keyword);
		return FALSE;
	}
	if(value == NULL)
	{
		DpRtLibrary_Error_Number = 19;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Double:value Pointer was NULL.\n");
		return FALSE;
	}
/* convert C to Java String */
	java_keyword_string = (*env)->NewStringUTF(env,keyword);
/* call getProperty method on DpRt_Status instance */
	(*value) = (double)((*env)->CallDoubleMethod(env,DpRt_Status,DpRt_Status_Get_Property_Double_Method_Id,
			java_keyword_string));
	return TRUE;
}

/**
 * Routine to get the boolean value of the keyword from the properties held in the instance of DpRtStatus.
 * @param keyword The keyword in the property file to look up.
 * @param value The address of an boolean to store the resulting value in.
 * @see #DpRt_Status
 * @see #DpRt_Status_Get_Property_Boolean_Method_Id
 */
static int DpRtLibrary_Get_Property_Boolean(char *keyword,int *value)
{
	JNIEnv *env = NULL;
	jstring java_keyword_string = NULL;
	jboolean boolean_value;

	if(DpRt_Status == NULL)
	{
		DpRtLibrary_Error_Number = 20;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Boolean:DpRt_Status was NULL(%s).\n",
			keyword);
		return FALSE;
	}
	if(DpRt_Status_Get_Property_Boolean_Method_Id == NULL)
	{
		DpRtLibrary_Error_Number = 21;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Boolean:Method ID was NULL(%s).\n",keyword);
		return FALSE;
	}
	if(Java_VM == NULL)
	{
		DpRtLibrary_Error_Number = 22;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Boolean:Java_VM was NULL(%s).\n",keyword);
		return FALSE;
	}
/* get java env for this thread */
	(*Java_VM)->AttachCurrentThread(Java_VM,(void**)&env,NULL);
	if(env == NULL)
	{
		DpRtLibrary_Error_Number = 23;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Boolean:env was NULL (%s).\n",keyword);
		return FALSE;
	}
	if(keyword == NULL)
	{
		DpRtLibrary_Error_Number = 24;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Boolean:string (%s) was NULL.\n",keyword);
		return FALSE;
	}
	if(value == NULL)
	{
		DpRtLibrary_Error_Number = 25;
		sprintf(DpRtLibrary_Error_String,"DpRtLibrary_Get_Property_Boolean:value Pointer was NULL.\n");
		return FALSE;
	}
/* convert C to Java String */
	java_keyword_string = (*env)->NewStringUTF(env,keyword);
/* call getProperty method on DpRt_Status instance */
	boolean_value = (double)((*env)->CallBooleanMethod(env,DpRt_Status,DpRt_Status_Get_Property_Boolean_Method_Id,
			java_keyword_string));
	if(boolean_value)
		(*value) = TRUE;
	else
		(*value) = FALSE;
	return TRUE;
}

/**
 * libdprt Log Handler for the Java layer interface. This calls the ngat.dprt.ccs.DpRtLibrary logger's 
 * log(int level,String message) method with the parameters supplied to this routine.
 * If the Logger instance is NULL, or the Log_Method_Id is NULL the call is not made.
 * Otherwise, A java.lang.String instance is constructed from the string parameter,
 * and the JNI CallVoidMEthod routine called to call log().
 * @param level The log level of the message.
 * @param string The message to log.
 * @see #java_vm
 * @see #Logger
 * @see #Log_Method_Id
 */
static void DpRtLibrary_Log_Handler(int level,char *string)
{
	JNIEnv *env = NULL;
	jstring java_string = NULL;

	if(Logger == NULL)
	{
		fprintf(stderr,"DpRtLibrary_Log_Handler:Logger was NULL (%d,%s).\n",level,string);
		return;
	}
	if(Log_Method_Id == NULL)
	{
		fprintf(stderr,"DpRtLibrary_Log_Handler:Log_Method_Id was NULL (%d,%s).\n",level,string);
		return;
	}
	if(Java_VM == NULL)
	{
		fprintf(stderr,"DpRtLibrary_Log_Handler:Java_VM was NULL (%d,%s).\n",level,string);
		return;
	}
/* get java env for this thread */
	(*Java_VM)->AttachCurrentThread(Java_VM,(void**)&env,NULL);
	if(env == NULL)
	{
		fprintf(stderr,"DpRtLibrary_Log_Handler:env was NULL (%d,%s).\n",level,string);
		return;
	}
	if(string == NULL)
	{
		fprintf(stderr,"DpRtLibrary_Log_Handler:string (%d) was NULL.\n",level);
		return;
	}
/* convert C to Java String */
	java_string = (*env)->NewStringUTF(env,string);
/* call log method on logger instance */
	(*env)->CallVoidMethod(env,Logger,Log_Method_Id,(jint)level,java_string);
}

/*
** $Log: not supported by cvs2svn $
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
