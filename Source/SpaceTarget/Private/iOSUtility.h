#pragma once

#include "CoreMinimal.h"
#include <Containers/UnrealString.h>
#include "SpaceTargetDefinition.h"
#include "SpaceTargetModule.h"
class iOSUtility
{
public:
#if PLATFORM_IOS
	static NSString* FStringToNSString(const FString& fstring);

	static FString NSStringToFString(const NSString* nsstring);
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
	static bool Relocate(FVector cam_positon, FQuat cam_rotation, FVector2D focal, FVector2D principal, FIntPoint resolution, uint8** cTexture, int length, int camStatus, FString aixs_x, FString aixs_y, FString aixs_z);
	//get the last relocated result.(JsonType)
	static bool GetResult(FString& recvStr);

	static void release();
};

