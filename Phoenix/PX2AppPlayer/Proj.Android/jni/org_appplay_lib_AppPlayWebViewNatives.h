/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_appplay_lib_AppPlayWebViewNatives */

#ifndef _Included_org_appplay_lib_AppPlayWebViewNatives
#define _Included_org_appplay_lib_AppPlayWebViewNatives
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    shouldStartLoading
 * Signature: (ILjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_shouldStartLoading
  (JNIEnv *, jclass, jint, jstring);

/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    didFinishLoading
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_didFinishLoading
  (JNIEnv *, jclass, jint, jstring);

/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    didFailLoading
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_didFailLoading
  (JNIEnv *, jclass, jint, jstring);

/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    onJsCallback
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_onJsCallback
  (JNIEnv *, jclass, jint, jstring);

/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    onSetByteArrayBuffer
 * Signature: (I[BI)V
 */
JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_onSetByteArrayBuffer
  (JNIEnv *, jclass, jint, jbyteArray, jint);

/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    didLoadHtmlSource
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_didLoadHtmlSource
  (JNIEnv *, jclass, jstring);

/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    pause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_pause
  (JNIEnv *, jclass);

/*
 * Class:     org_appplay_lib_AppPlayWebViewNatives
 * Method:    resume
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_appplay_lib_AppPlayWebViewNatives_resume
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
