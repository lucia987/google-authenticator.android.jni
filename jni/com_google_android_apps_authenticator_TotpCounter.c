#include "com_google_android_apps_authenticator_TotpCounter.h"

/*
 * ORIGINAL JAVA CODE:
 *  long timeSinceStartTime = time - mStartTime;
    if (timeSinceStartTime >= 0) {
      return timeSinceStartTime / mTimeStep;
    } else {
      return (timeSinceStartTime - (mTimeStep - 1)) / mTimeStep;
    }
 */
JNIEXPORT jlong JNICALL Java_com_google_android_apps_authenticator_TotpCounter_getValueAtTimeNative
  (JNIEnv *pEnv, jobject pObj, jlong startTime, jlong timeStep, jlong time) {
	jlong timeSinceStartTime = time - startTime;

	if (timeSinceStartTime >= 0) {
		return timeSinceStartTime / timeStep;
	} else {
		return (timeSinceStartTime - (timeStep - 1)) / timeStep;
	}
}
