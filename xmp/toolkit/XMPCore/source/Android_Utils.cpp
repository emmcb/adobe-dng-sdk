// =================================================================================================
// Copyright 2007 Adobe
// All Rights Reserved.
//
// NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance with the terms
// of the Adobe license agreement accompanying it. If you have received this file from a source other 
// than Adobe, then your use, modification, or distribution of it requires the prior written permission
// of Adobe.
// =================================================================================================
#include "XMPCore/source/Android_Utils.hpp"
#include "XMPCore/source/XMPDocOps.hpp"
#if XMP_AndroidBuild
// TODO(erichan): removed
/*
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){

    cachedJVM =vm;
    return JNI_VERSION_1_6;
} */

/* class static */ void
XMPDocOps::SetJVM ( JavaVM* vm )
{
	/*
	 * This function is now enabled. As now we'd have stopped using boost to generate uuid
	 */
    if(vm)
        cachedJVM = vm;

}	// SetJVM

void GuidGenerationUsingJNI(JNIEnv** p_env, BinaryID &binID  ){
	(*p_env)->PushLocalFrame(10);
    jclass uuidClass = (*p_env)->FindClass("java/util/UUID");

    jmethodID newGuidMethod = (*p_env)->GetStaticMethodID(uuidClass, "randomUUID", "()Ljava/util/UUID;");
    jmethodID mostSignificantBitsMethod = (*p_env)->GetMethodID(uuidClass, "getMostSignificantBits", "()J");
    jmethodID leastSignificantBitsMethod = (*p_env)->GetMethodID(uuidClass, "getLeastSignificantBits", "()J");


    jobject javaUuid = (*p_env)->CallStaticObjectMethod(uuidClass, newGuidMethod);

    jlong mostSignificant = (*p_env)->CallLongMethod(javaUuid, mostSignificantBitsMethod);
    jlong leastSignificant = (*p_env)->CallLongMethod(javaUuid, leastSignificantBitsMethod);

    binID[0]=(XMP_Uns8)((mostSignificant >> 56) & 0xFF);
    binID[1]=(XMP_Uns8)((mostSignificant >> 48) & 0xFF);
    binID[2]=(XMP_Uns8)((mostSignificant >> 40) & 0xFF);
    binID[3]=(XMP_Uns8)((mostSignificant >> 32) & 0xFF);
    binID[4]=(XMP_Uns8)((mostSignificant >> 24) & 0xFF);
    binID[5]=(XMP_Uns8)((mostSignificant >> 16) & 0xFF);
    binID[6]=(XMP_Uns8)((mostSignificant >> 8) & 0xFF);
    binID[7]=(XMP_Uns8)(mostSignificant & 0xFF);
    binID[8]=(XMP_Uns8)((leastSignificant >> 56) & 0xFF);
    binID[9]=(XMP_Uns8)((leastSignificant >> 48) & 0xFF);
    binID[10]=(XMP_Uns8)((leastSignificant >> 40) & 0xFF);
    binID[11]=(XMP_Uns8)((leastSignificant >> 32) & 0xFF);
    binID[12]=(XMP_Uns8)((leastSignificant >> 24) & 0xFF);
    binID[13]=(XMP_Uns8)((leastSignificant >> 16) & 0xFF);
    binID[14]=(XMP_Uns8)((leastSignificant >> 8) & 0xFF);
    binID[15]=(XMP_Uns8)(leastSignificant & 0xFF);

    (*p_env)->PopLocalFrame(NULL);
};
void GuidGenerator(BinaryID &binID )
{

    JNIEnv* env;
    int doDetachThread=0;

    if(!cachedJVM)
        XMP_Throw ( "JVM not accessible to native process", kXMPErr_ExternalFailure );

    if (cachedJVM->GetEnv (reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
        GuidGenerationUsingJNI(&env,binID);

    } else {
        if(cachedJVM->AttachCurrentThread( &env, NULL) == JNI_OK)
            GuidGenerationUsingJNI(&env,binID);
        else
            XMP_Throw ( "Not able to AttachCurrentThread to JVM", kXMPErr_ExternalFailure );

        doDetachThread=1;
    }


    if(doDetachThread)
        cachedJVM->DetachCurrentThread();

}

#endif //XMP_AndroidBuild
