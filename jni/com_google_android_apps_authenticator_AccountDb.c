#include "com_google_android_apps_authenticator_AccountDb.h"
#include <openssl/hmac.h>
#include <android/log.h>

JNIEXPORT jbyteArray JNICALL Java_com_google_android_apps_authenticator_AccountDb_signNative
  (JNIEnv *pEnv, jclass pObj, jbyteArray pKeyArray, jbyteArray pDataArray) {
	jsize lKeyLength, lDataLength;
	char *lKeyBytes, *lDataBytes;


	/* Java jbyteArrays to C byte */
	lKeyLength = (*pEnv)->GetArrayLength(pEnv, pKeyArray);
	lKeyBytes = (char *)malloc(lKeyLength * sizeof(char));
	(*pEnv)->GetByteArrayRegion(pEnv, pKeyArray, 0, lKeyLength, lKeyBytes);
	if ((*pEnv)->ExceptionCheck(pEnv)) {
		(*pEnv)->ExceptionDescribe(pEnv);
		__android_log_print(ANDROID_LOG_ERROR, "AccountDb_signNative", "GetByteArrayRegion failed for key");
		return NULL;
	}

	lDataLength = (*pEnv)->GetArrayLength(pEnv, pDataArray);
	lDataBytes = (char *)malloc(lDataLength * sizeof(char));
	(*pEnv)->GetByteArrayRegion(pEnv, pDataArray, 0, lDataLength, lDataBytes);
	if ((*pEnv)->ExceptionCheck(pEnv)) {
		(*pEnv)->ExceptionDescribe(pEnv);
		__android_log_print(ANDROID_LOG_ERROR, "AccountDb_signNative", "GetByteArrayRegion failed for data");
		return NULL;
	}

	/* START OF code for TEE */
	char *lResultBytes;
	unsigned int lResultLen = lDataLength > lKeyLength ?
			lDataLength : lKeyLength;
	lResultBytes = (char *)calloc(lResultLen, sizeof(char));
	if (lResultBytes == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, "AccountDb_signNative", "cannot calloc lResultArray");
		return NULL;
	}


	// Initialise HMAC_CTX  before it can be used
	HMAC_CTX ctx;
	HMAC_CTX_init(&ctx);

	// Reuse HMAC_CTX to use hash function EVP_sha1() and lKeyBytes as key
	HMAC_Init_ex(&ctx, lKeyBytes, lKeyLength, EVP_sha1(), NULL);
	// Can be called repeatedly with chunks of the message to be authenticated
	HMAC_Update(&ctx, (unsigned char*)lDataBytes, lDataLength);
	// Place the hash function output in lResultBytes which must have eough space for the output
	HMAC_Final(&ctx, (unsigned char *)lResultBytes, &lResultLen);

	HMAC_CTX_cleanup(&ctx);
	/* END OF code for TEE */

	/* C byte* to Java jbyteArray */
	jbyteArray lResultArray = (*pEnv)->NewByteArray(pEnv, lResultLen);
	if (lResultArray == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, "AccountDb_signNative", "NewByteArray failed for lResultArray");
		return NULL;
	}

	(*pEnv)->SetByteArrayRegion(pEnv, lResultArray, 0, lResultLen, lResultBytes);
	free(lResultBytes);

	return lResultArray;
}
