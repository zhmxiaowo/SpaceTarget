#pragma once


#if PLATFORM_ANDROID
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#endif
#include "CoreMinimal.h"
#include <Containers/UnrealString.h>
#include "SpaceTargetDefinition.h"

class AndroidUtility
{
public:
#if PLATFORM_ANDROID
	static jclass relocateClassID;
	static jobject relocateObjectID;
	static jmethodID relocateInitID;
	static jmethodID relocateID;
	static jmethodID relocateResultID;

	static jbyteArray ConvertToJByteArray(const uint8* byteArray, int32 size);
	static jfloatArray  ConvertToJFloatArray(const TArray<float>& floatArray);
	static jintArray ConvertToJIntArray(const TArray<int>& intArray);
	static FString FromJavaFString(jstring javaString);
	static jstring GetJavaString(FString string);
#endif
public:
	//init algorithm running path
	static bool Init(FString& path);
	//run a frame of relocation.
	/// <param name="cam_positon">current slam camera position</param>
	/// <param name="cam_rotation">current slam camera rotation</param>
	/// <param name="focal">camera focal</param>
	/// <param name="priciple">camera priciple</param>
	/// <param name="resolution">the cTexture image resolution,example 640x480</param>
	/// <param name="cTexture">camera texture data,fomat is RGBA8888</param>
	/// <param name="length">cTexture length</param>
	/// <returns></returns>
	static bool Relocate(FVector cam_positon,FQuat cam_rotation,FVector2D focal,FVector2D principal,FIntPoint resolution,uint8** cTexture,int length, int method, FString aixs_x, FString aixs_y, FString aixs_z,int camStatus);
	//get the last relocated result.(JsonType)
	static bool GetResult(FString& recvStr);
};

