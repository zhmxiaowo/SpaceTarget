#include "AndroidUtility.h"

#if PLATFORM_ANDROID

jclass AndroidUtility::relocateClassID = NULL;

jobject AndroidUtility::relocateObjectID = NULL;

jmethodID AndroidUtility::relocateInitID = NULL;

jmethodID AndroidUtility::relocateID = NULL;

jmethodID AndroidUtility::relocateResultID = NULL;


jbyteArray AndroidUtility::ConvertToJByteArray(const uint8* byteArray, int32 size)
{
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    jbyteArray javaByteArray = (jbyteArray)Env->NewByteArray(size);
    jbyte* javaByteArrayPtr = (jbyte*)malloc(size * sizeof(jbyte));
    for (int i = 0; i < size; i++) {
        javaByteArrayPtr[i] = byteArray[i];
    }
    Env->SetByteArrayRegion(javaByteArray, 0, size, javaByteArrayPtr);
    free(javaByteArrayPtr);
    return javaByteArray;
}

jfloatArray AndroidUtility::ConvertToJFloatArray(const TArray<float>& floatArray)
{
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    jfloatArray javaFloatArray = (jfloatArray)Env->NewFloatArray(floatArray.Num());
    jfloat* javaFloatArrayPtr = (jfloat*)malloc(floatArray.Num() * sizeof(jfloat));
    for (int i = 0; i < floatArray.Num(); i++) {
        javaFloatArrayPtr[i] = floatArray[i];
    }
    Env->SetFloatArrayRegion(javaFloatArray, 0, floatArray.Num(), javaFloatArrayPtr);
    free(javaFloatArrayPtr);
    return javaFloatArray;
}

jintArray AndroidUtility::ConvertToJIntArray(const TArray<int>& intArray)
{
    JNIEnv* Env = FAndroidApplication::GetJavaEnv();
    jintArray javaIntArray = (jintArray)Env->NewIntArray(intArray.Num());
    jint* javaIntArrayPtr = (jint*)malloc(intArray.Num() * sizeof(jint));
    for (int i = 0; i < intArray.Num(); i++) {
        javaIntArrayPtr[i] = (jint)intArray[i];
    }
    Env->SetIntArrayRegion(javaIntArray, 0, intArray.Num(), javaIntArrayPtr);
    free(javaIntArrayPtr);
    return javaIntArray;
}

FString AndroidUtility::FromJavaFString(jstring javaString)
{
    JNIEnv* Env = AndroidJavaEnv::GetJavaEnv();
    const char* UTFString = Env->GetStringUTFChars(javaString, nullptr);
    FString Result(UTF8_TO_TCHAR(UTFString));
    Env->ReleaseStringUTFChars(javaString, UTFString);
    Env->DeleteLocalRef(javaString);

    return Result;
}

jstring AndroidUtility::GetJavaString(FString string)
{
    JNIEnv* JEnv = AndroidJavaEnv::GetJavaEnv();
    jstring local = JEnv->NewStringUTF(TCHAR_TO_UTF8(*string));
    jstring result = (jstring)JEnv->NewGlobalRef(local);
    JEnv->DeleteLocalRef(local);
    return result;
}
#endif
bool AndroidUtility::Init(FString& path)
{
    bool isCallSuccess = false;
#if PLATFORM_ANDROID
    //pure jni call
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        bool bIsOptional = true;
        relocateClassID = FAndroidApplication::FindJavaClassGlobalRef("com/fdage/ar/AR_Relocate_Core_Api");

        relocateObjectID = Env->NewObject(relocateClassID, Env->GetMethodID(relocateClassID, "<init>", "()V"));
        //need to delete on ~()
        relocateObjectID = Env->NewGlobalRef(relocateObjectID);
        relocateID = FJavaWrapper::FindStaticMethod(Env, relocateClassID, "imgLocateProcess_unreal_argb32", "([F[F[F[F[I[BILjava/lang/String;Ljava/lang/String;Ljava/lang/String;II)Z", bIsOptional);
        relocateInitID = FJavaWrapper::FindStaticMethod(Env, relocateClassID, "init", "(Landroid/content/Context;Ljava/lang/String;)Z", bIsOptional);
        relocateResultID = FJavaWrapper::FindStaticMethod(Env, relocateClassID, "get_sfm_result", "()Ljava/lang/String;", bIsOptional);
        //get the android context is complex.we use UPL write a simple method 'Context ARContext()' to get it.
        jobject contextID = FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "ARContext", "()Landroid/content/Context;", bIsOptional));

        jstring jstr = GetJavaString(path);

        isCallSuccess = (bool)Env->CallStaticBooleanMethod(relocateClassID, relocateInitID, contextID, jstr);
        //if we use va_list ,when it calculate jobject memery size will make mistake.so we use ↑ original method to call it.
        //bool b = FJavaWrapper::CallBooleanMethod(Env, relocateObjectID, relocateInitID, *contextID, *jstr);
        Env->DeleteGlobalRef(jstr);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (isCallSuccess ? FString(TEXT("init sfm success")) : FString(TEXT("init sfm failure"))));
    }
    //use main activity
    //if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    //{
    //    relocateInitID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "init", "(Ljava/lang/String;)Z", bIsOptional);
    //    relocateID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "imgLocateProcess_unreal", "([F[F[F[F[I[BI)Z", bIsOptional);
    //    relocateResultID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "get_sfm_result", "()Ljava/lang/String;", bIsOptional);

    //    FString datapath = FPaths::ProjectPersistentDownloadDir() + "/" + UARBlueprintFunctionLibrary::GetARBaseDataID();
    //    auto jstr = FJavaHelper::ToJavaString(Env, datapath);

    //    bool b = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, relocateInitID, *jstr);
    //    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (b ? FString(TEXT("init sfm success")) : FString(TEXT("init sfm failure"))));
    //}
#else
    isCallSuccess = true;
#endif
    return isCallSuccess;
}

bool AndroidUtility::Relocate(FVector cam_positon,FQuat cam_rotation, FVector2D focal, FVector2D principal, FIntPoint resolution, uint8** cTexture, int length,int method,FString aixs_x,FString aixs_y,FString aixs_z,int camStatus)
{
    bool isCallSuccess = false;

    int decrimel = 6;
    UE_LOG(LogTemp, Warning, TEXT("focal: %s pricipal: %s  resolution: %s "), *(focal.ToString()), *(principal.ToString()), *(resolution.ToString()));
    UE_LOG(LogTemp, Warning, TEXT("camera position: %s camera rotation: %s "), *(cam_positon.ToString()), *(cam_rotation.ToString()));
    TArray<float> _v = { cam_positon.X,cam_positon.Y,cam_positon.Z, };
    TArray<float> _q = { cam_rotation.X,cam_rotation.Y,cam_rotation.Z,cam_rotation.W, };
    TArray<float> _focal = { focal.X,focal.Y, };
    TArray<float> _principal = { principal.X,principal.Y, };
    TArray<int> _resolution = { resolution.X,resolution.Y };
    //TArray<float> _v = { DECIMALFLOAT(cam_positon.X, decrimel),DECIMALFLOAT(cam_positon.Y, decrimel),DECIMALFLOAT(cam_positon.Z, decrimel) };
    //TArray<float> _q = { DECIMALFLOAT(cam_rotation.X, decrimel) ,DECIMALFLOAT(cam_rotation.Y, decrimel),DECIMALFLOAT(cam_rotation.Z, decrimel) ,DECIMALFLOAT(cam_rotation.W, decrimel) };
    //TArray<float> _focal = { DECIMALFLOAT(focal.X, decrimel),DECIMALFLOAT(focal.Y,  decrimel) };
    //TArray<float> _principal = { DECIMALFLOAT(principal.X, decrimel),DECIMALFLOAT(principal.Y, decrimel) };
    //TArray<int> _resolution = { resolution.X,resolution.Y };

#if PLATFORM_ANDROID
    //----------------------------------------------c++ to java propety
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {

        jfloatArray j_v = ConvertToJFloatArray(_v);
        jfloatArray j_q = ConvertToJFloatArray(_q);
        jfloatArray j_focal = ConvertToJFloatArray(_focal);
        jfloatArray j_principal = ConvertToJFloatArray(_principal);
        jintArray   j_resolution = ConvertToJIntArray(_resolution);

        int32 size = length;
        uint8* texPtr = *cTexture;
        jbyteArray j_bytes = ConvertToJByteArray(texPtr, size);

        jint j_method = (jint)method;

        jstring j_x = GetJavaString(aixs_x);
        jstring j_y = GetJavaString(aixs_y);
        jstring j_z = GetJavaString(aixs_z);

        //ue4 : 1,  unity : 0
        jint j_platform = (jint)1;
        jint j_state = (jint)camStatus;

        isCallSuccess = Env->CallStaticBooleanMethod(relocateClassID, relocateID, j_v, j_q, j_focal, j_principal, j_resolution, j_bytes, j_method, j_x, j_y, j_z, j_platform, j_state);
        //isCallSuccess = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, relocateID, j_v, j_q, j_focal, j_pricipal, j_resolution, j_bytes, j_state);
        UE_LOG(LogTemp, Warning, TEXT("finish call relocate"));
        Env->DeleteLocalRef(j_bytes);
        Env->DeleteLocalRef(j_v);
        Env->DeleteLocalRef(j_q);
        Env->DeleteLocalRef(j_focal);
        Env->DeleteLocalRef(j_principal);
        Env->DeleteLocalRef(j_resolution);

        Env->DeleteGlobalRef(j_x);
        Env->DeleteGlobalRef(j_y);
        Env->DeleteGlobalRef(j_z);

        //if (isCallSuccess)
        //{
        //    UE_LOG(LogTemp, Warning, TEXT("AndroidUtility::Relocate success!!"));
        //}
        //else
        //{
        //    UE_LOG(LogTemp, Warning, TEXT("AndroidUtility::Relocate failure!!"));
        //}
    }
#endif


    return isCallSuccess;
}

bool AndroidUtility::GetResult(FString& recvStr)
{
    bool isCallSuccess = false;
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        //jstring jstr = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, relocateResultID);
        jstring jstr = (jstring)Env->CallStaticObjectMethod(relocateClassID, relocateResultID);
        recvStr = FJavaHelper::FStringFromParam(Env, jstr);
        Env->DeleteLocalRef(jstr);
        isCallSuccess = true;
    }
#endif
    return isCallSuccess;
}