#include "ARCoreSlamGenerator.h"

#if PLATFORM_ANDROID
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#include "GoogleARCoreCameraImage.h"
#include "GoogleARCoreTypes.h"
#include "GoogleARCoreFunctionLibrary.h"
#include "GoogleARCoreCameraIntrinsics.h"
#endif
#include <ARBlueprintLibrary.h>
#include "SpaceTargetModule.h"

AARCoreSlamGenerator::AARCoreSlamGenerator()
{
	isInitIntrinsics = false;
}

void AARCoreSlamGenerator::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogSpaceTarget, Log, TEXT("ARCoreSlamGenerator has been created!"));
}

void AARCoreSlamGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//#include <Engine/World.h>

bool AARCoreSlamGenerator::GetCameraIntrinsics(FCameraIntrisics& ci)
{
	bool result = false;

	//调用太频繁直接崩溃了,测试后发现参数都是一样的,所以缓存起来
	if(!isInitIntrinsics)
	{
#if PLATFORM_ANDROID
		UGoogleARCoreCameraIntrinsics* cameraintrinsics = nullptr;
		EGoogleARCoreFunctionStatus google_intrinsics_status = UGoogleARCoreFrameFunctionLibrary::GetCameraImageIntrinsics(cameraintrinsics);
		if (google_intrinsics_status == EGoogleARCoreFunctionStatus::Success)
		{
			cameraintrinsics->GetImageDimensions(_intrisics.resolution.X, _intrisics.resolution.Y);
			cameraintrinsics->GetFocalLength(_intrisics.focal.X, _intrisics.focal.Y);
			cameraintrinsics->GetPrincipalPoint(_intrisics.pricipal.X, _intrisics.pricipal.Y);

			if(_intrisics.resolution.X != 0 && _intrisics.resolution.Y != 0)
			{
				UE_LOG(LogSpaceTarget,Log,TEXT("_intrisics : %s"),*(_intrisics.resolution.ToString()));
				isInitIntrinsics = true;
				result = true;
			}
		}
		else
		{
			result = false;
		}
#endif
	}
	else
	{
		ci = _intrisics;
		result = true;
	}
	return result;
}

bool AARCoreSlamGenerator::GetCameraPose(FCameraPose& cp)
{
	bool result = false;

    UWorld* world = this->GetWorld();
	if(world)
	{
		APlayerCameraManager* camManager = world->GetFirstPlayerController()->PlayerCameraManager;
		if(camManager)
		{
			FVector cam_pos = camManager->GetCameraLocation();
			FQuat cam_qua = camManager->GetCameraRotation().Quaternion();
			UE_LOG(LogSpaceTarget, Log, TEXT("cam_pos : %s  cam_qua : %s"), *(cam_pos.ToString()), *(cam_qua.ToString()));
			cp.position = cam_pos;
			cp.rotation = cam_qua;
			if(cp.position.ContainsNaN())
			{
				result = false;
			}else
			{
				result = true;
			}

		}
	}
	return result;
}

void AARCoreSlamGenerator::SetCameraPose(FTransform cp)
{
	//TODO:存在问题,需要改
	UWorld* world = this->GetWorld();
	if (world)
	{
		APlayerCameraManager* camManager = world->GetFirstPlayerController()->PlayerCameraManager;
		if (camManager)
		{
			FVector InCamera_v = camManager->GetCameraLocation();
			FRotator InCamera_r = camManager->GetCameraRotation();
			FVector v = cp.GetLocation();
			FRotator r = cp.GetRotation().Rotator();

			FVector CorrectVector = FRotator(0, InCamera_r.Yaw - r.Yaw, 0).RotateVector(v);

			FVector camera_v = InCamera_v + v;
			FRotator camera_r = InCamera_r + r;

			UARBlueprintLibrary::SetARWorldOriginLocationAndRotation(InCamera_v - CorrectVector, FRotator(0, InCamera_r.Yaw - r.Yaw, 0), true, true);


			//FTransform alignTransform = UARBlueprintLibrary::GetAlignmentTransform();
			//FTransform orignal = cp * camManager->GetActorTransform();
			//orignal = alignTransform * orignal;
			//FRotator rotator = orignal.Rotator();
			//rotator.Pitch = 0;
			//rotator.Roll = 0;
			//orignal.SetRotation(rotator.Quaternion());
			//UARBlueprintLibrary::SetAlignmentTransform(orignal);
			//UARBlueprintLibrary::SetARWorldOriginLocationAndRotation(orignal.GetLocation(), orignal.Rotator(), true, true);
		}
	}
	//UARBlueprintLibrary::SetARWorldOriginLocationAndRotation(cp.GetLocation(), cp.Rotator(), true, true);

}


bool AARCoreSlamGenerator::GetTrackingStatus(FXRTrackingState& ts)
{
	bool result = false;

    if (UARBlueprintLibrary::GetARSessionStatus().Status == EARSessionStatus::Running)
    {
        EARTrackingQuality currTrackingQuality = UARBlueprintLibrary::GetTrackingQuality();
        switch (currTrackingQuality)
        {
        case EARTrackingQuality::NotTracking:
            ts = FXRTrackingState::Lost;
            break;
        case EARTrackingQuality::OrientationAndPosition:
            ts = FXRTrackingState::Tracking;
            break;
        case EARTrackingQuality::OrientationOnly:
            ts = FXRTrackingState::TrackingUnStable;
            break;
        default:
            ts = FXRTrackingState::Lost;
            break;
        }
		result = true;
	}
	else
	{
		ts = FXRTrackingState::None;
	}
	return result;
}

bool AARCoreSlamGenerator::GetCameraTexture(FCameraTexture& ct)
{
	bool result = false;
	if (UARBlueprintLibrary::GetARSessionStatus().Status == EARSessionStatus::Running)
	{
#if PLATFORM_ANDROID
		UGoogleARCoreCameraImage* cameraimage = nullptr;

		EGoogleARCoreFunctionStatus google_image_status = UGoogleARCoreFrameFunctionLibrary::AcquireCameraImage(cameraimage);
		if (google_image_status == EGoogleARCoreFunctionStatus::Success)
		{

			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("get camera image success")));
			int32 pixelStride = 0;
			int32 rowStride = 0;

			const uint8* ydata = nullptr;
			const uint8* udata = nullptr;
			const uint8* vdata = nullptr;
			int32 ylen = 0, ulen = 0, vlen = 0;

			int32 width = cameraimage->GetWidth();
			int32 height = cameraimage->GetHeight();

			udata = cameraimage->GetPlaneData(1, pixelStride, rowStride, ulen);
			//ArImage_getPlaneData(arsession, cameraimage->ArImage, 1, &udata, ulen);

			vdata = cameraimage->GetPlaneData(2, pixelStride, rowStride, vlen);
			//ArImage_getPlaneData(cameraimage->SessionHandle,cameraimage->ArImage, 2, &vdata, vlen);

			ydata = cameraimage->GetPlaneData(0, pixelStride, rowStride, ylen);
			//ArImage_getPlaneData(cameraimage->SessionHandle, cameraimage->ArImage, 0, &ydata, ylen);

			if (ylen == 0 || ulen == 0)
			{
				UE_LOG(LogSpaceTarget, Error , TEXT("yuv data error"));
				return result;
			}

			int mem_len = width * height * 4;
			if (ct.data == NULL || ct.length != mem_len)
			{
				delete(ct.data);
				ct.data = new uint8[mem_len];
				ct.length = mem_len;
			}

			//yuv420 to argb
			int r, g, b;
			uint8* rptr = ct.data;
			for (size_t y = 0; y < height; y++)
			{
				const uint8* Y_rowPtr = ydata + y * rowStride;
				const uint8* UV_rowPtr = udata + (y >> 1) * rowStride;
				// uint8_t * V_rowPtr = srcVPtr + (y >> 1) * Y_rowStride / 4;
				for (size_t x = 0; x < width; x++)
				{
					uint8_t Y = Y_rowPtr[x];
					size_t uIndex = x & 0xfffffffe;
					uint8_t U = UV_rowPtr[uIndex];
					uint8_t V = UV_rowPtr[uIndex + 1];
					double R = ((Y - 16) * 1.164 + (V - 128) * 1.596);
					double G = ((Y - 16) * 1.164 - (U - 128) * 0.392 - (V - 128) * 0.813);
					double B = ((Y - 16) * 1.164 + (U - 128) * 2.017);
					r = FMath::Clamp((int)R, 0, 255);
					g = FMath::Clamp((int)G, 0, 255);
					b = FMath::Clamp((int)B, 0, 255);
					*(rptr++) = 255; // alpha always 255
					*(rptr++) = (uint8)r;
					*(rptr++) = (uint8)g;
					*(rptr++) = (uint8)b;
				}
			}
			ydata = nullptr;
			udata = nullptr;
			vdata = nullptr;
			cameraimage->Release();
			//ArImage_release(cameraimage);

			//android ndk image direction is left.
			ct.direction = FImageDirection::LEFT;

			result = true;
		}
		else
		{
			//fail to get image ,mostly is the ARCore not avilable.
			UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGoogleARCoreFunctionStatus"), true);
			FString CurShooterDataStr(EnumPtr->GetNameByValue((int)google_image_status).ToString());
			UE_LOG(LogSpaceTarget, Error, TEXT("EGoogleARCoreFunctionStatus : %s"), *CurShooterDataStr);
			return result;
		}
#endif
	}
	return result;
}
