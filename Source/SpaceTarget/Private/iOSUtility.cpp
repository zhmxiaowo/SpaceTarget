#include "iOSUtility.h"
#include <sstream>

#if PLATFORM_IOS
#include <FKARSFM/AR_core_interface.h>
#endif

#if PLATFORM_IOS
NSString* iOSUtility::FStringToNSString(const FString& fstring)
{
	return [NSString stringWithUTF8String : TCHAR_TO_UTF8(*fstring)];
}

FString iOSUtility::NSStringToFString(const NSString* nsstring)
{
	if (nsstring != nullptr &&
		[nsstring length] > 0)
	{
		return UTF8_TO_TCHAR(std::string(nsstring.UTF8String).c_str());
	}

	return FString{};
}
#endif



bool iOSUtility::Init(FString& path)
{
	bool result = false;
#if PLATFORM_IOS
	//int iosreturn = _iOS_testhaha();
	_iOS_setDebug(1);
	//_iOS_setUE4DebugPath(1);
	//NSString* nspath = FStringToNSString(path);
	//result = [[AR_core_interface shareSingleton] creatOperationPath:nspath sfmARBinFilePath:@"" sfmARDescFilePath:@"" sfmARfeatFilePath:@""];
	//UE_LOG(LogSpaceTarget,Log,TEXT("ios init : %s"),*(result?FString("true"):FString("false")));
	FString path_absolute = SpaceTargetDefinition::AbsolutePath(path);
	std::string std_cstr = std::string(TCHAR_TO_UTF8(*path_absolute));
	char* cstr = const_cast<char*>(std_cstr.c_str());
	//char* cstr = TCHAR_TO_ANSI(*path_absolute);
	FString tfpath = SpaceTargetDefinition::AbsolutePath(path + SpaceTargetDefinition::enctflite);
	std::string std_tfpath = std::string(TCHAR_TO_UTF8(*tfpath));
	char* tfstr = const_cast<char*>(std_tfpath.c_str());
	//char* tfstr = TCHAR_TO_ANSI(*tfpath);
	UE_LOG(LogSpaceTarget, Log, TEXT("FKARSFM framework ios path_absolute: %s"), *path_absolute);
	result = _iOS_init_FKARSFM(cstr, tfstr, cstr, cstr);
	UE_LOG(LogSpaceTarget, Log, TEXT("FKARSFM framework ios tfpath : %s"), *tfpath);
#endif
	return result;
}

bool iOSUtility::Relocate(FVector cam_positon, FQuat cam_rotation, FVector2D focal, FVector2D principal, FIntPoint resolution, uint8** cTexture, int length, int camStatus, FString aixs_x, FString aixs_y, FString aixs_z)
{
	bool result = false;
#if PLATFORM_IOS
	AR_SFM_Point c_cam_pos = { cam_positon.X,cam_positon.Y,cam_positon.Z };
	AR_SFM_rotation c_cam_qua = { cam_rotation.X,cam_rotation.Y, cam_rotation.Z, cam_rotation.W};
	AR_SFM_focalLength c_focal = { focal.X,focal.Y};
	UE_LOG(LogSpaceTarget, Log, TEXT("FKARSFM framework 11111 "));
	AR_SFM_principalPoint c_principal = { principal.X,principal.Y};
	AR_SFM_resolution c_resolution = { resolution.X,resolution.Y };
	long c_length = length;
	Byte* c_cTexture = (Byte*)(*cTexture);

	std::string std_x = std::string(TCHAR_TO_UTF8(*aixs_x));
	std::string std_y = std::string(TCHAR_TO_UTF8(*aixs_y));
	std::string std_z = std::string(TCHAR_TO_UTF8(*aixs_z));
	//char* cstr = const_cast<char*>(std_cstr.c_str());

	char* c_axis_x = const_cast<char*>(std_x.c_str());
	char* c_axis_y = const_cast<char*>(std_y.c_str());
	char* c_axis_z = const_cast<char*>(std_z.c_str());

	BOOL result_bool = _iOS_process_FKARSFM(c_cam_pos,c_cam_qua,c_focal, c_principal,c_resolution,camStatus, c_cTexture, c_length, c_axis_x, c_axis_y, c_axis_z, 1);//ue4: 1,   unity: 0
	result = (result_bool == YES);
	UE_LOG(LogSpaceTarget, Log, TEXT("FKARSFM framework 222222 %s   %c"),*(result?FString("true"):FString("false")), result_bool);
#endif
	return result;
}

bool iOSUtility::GetResult(FString& recvStr)
{
	bool result = false;
#if PLATFORM_IOS
	char* cstr = _iOS_get_result_FKARSFM();
	std::string std_cstr(cstr);
	if(strlen(cstr) > 0)
	{
		recvStr = FString(std_cstr.c_str());
		result = true;
	}
	UE_LOG(LogSpaceTarget, Log, TEXT("FKARSFM framework 3333333 %s"),*recvStr);
	free(cstr);
#endif
	return result;
}

void iOSUtility::release()
{
#if PLATFORM_IOS
	_iOS_close_FKARSFM();
#endif
}
