#include "ARKitSlamGenerator.h"
#include <ARBlueprintLibrary.h>
#include "SpaceTargetModule.h"

#include "ARBlueprintLibrary.h"
#include "SPARBaseAsyncTaskBlueprintProxy.h"
#if PLATFORM_IOS
#import <ARKit/ARKit.h>
//#include "AppleARKitSessionDelegate.h"
//#include "ARKitCoachingOverlay.h"
#endif

AARKitSlamGenerator::AARKitSlamGenerator()
{
	isInitIntrinsics = false;
}

void AARKitSlamGenerator::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogSpaceTarget, Log, TEXT("ARKitSlamGenerator has been created!"));
}

void AARKitSlamGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AARKitSlamGenerator::GetCameraIntrinsics(FCameraIntrisics& ci)
{
	bool result = false;
#if PLATFORM_IOS
	//auto ARSystem = FAppleARKitModule::GetARKitSystem();
#endif
	FARCameraIntrinsics OutCameraIntrinsics;
	if (UARBlueprintLibrary::GetCameraIntrinsics(OutCameraIntrinsics))
	{
		ci.focal = OutCameraIntrinsics.FocalLength;
		ci.pricipal = OutCameraIntrinsics.PrincipalPoint;
		ci.resolution = OutCameraIntrinsics.ImageResolution;
		UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 2 ! ci.resolution: %s !"), *(ci.resolution.ToString()));
		result = true;
	}
	return result;
}

bool AARKitSlamGenerator::GetCameraPose(FCameraPose& cp)
{
	bool result = false;

	//USpaceTargetSubsystem::get
	UWorld* world = this->GetWorld();
	if (world)
	{
		APlayerCameraManager* camManager = world->GetFirstPlayerController()->PlayerCameraManager;
		if (camManager)
		{
			FVector cam_pos = camManager->GetCameraLocation();
			FQuat cam_qua = camManager->GetCameraRotation().Quaternion();
			UE_LOG(LogSpaceTarget, Log, TEXT("cam_pos : %s  cam_qua : %s"), *(cam_pos.ToString()), *(cam_qua.ToString()));
			cp.position = cam_pos;
			cp.rotation = cam_qua;
			if (cp.position.ContainsNaN())
			{
				result = false;
			}
			else
			{
				result = true;
			}
		}
	}
	return result;
}

void AARKitSlamGenerator::SetCameraPose(FTransform cp)
{
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
			//FTransform orignal = camManager->GetActorTransform() * alignTransform.Inverse();
			//UARBlueprintLibrary::SetARWorldOriginLocationAndRotation(orignal.GetLocation(), orignal.Rotator(), true, true);
		}
	}
}

bool AARKitSlamGenerator::GetTrackingStatus(FXRTrackingState& ts)
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

bool AARKitSlamGenerator::GetCameraTexture(FCameraTexture& ct)
{
    bool result = false;
    volatile bool isrunning = true;

    TWeakPtr<FARSupportInterface, ESPMode::ThreadSafe> arSystem = USPARBaseAsyncTaskBlueprintProxy::GetARSystemPublic();
    void* arSession = nullptr;
    if (arSystem.IsValid())
    {
        arSession = arSystem.Pin()->GetARSessionRawPointer();
#if PLATFORM_IOS
        ARSession* nativeARSession = (ARSession*)arSession;
        
        //两个Frame是一样的
        ARFrame* nativeFrame = (ARFrame*)arSystem.Pin()->GetGameThreadARFrameRawPointer();
//        ARFrame* nativeFrame2 = nativeARSession.currentFrame;
        UE_LOG(LogSpaceTarget, Log, TEXT("processImageBuffer() enter~~~~"));
        CVPixelBufferRef imageBuffer = nativeFrame.capturedImage;

        //判断buffer的类型， YUV的还是 RGB的。
        //iOS ARKit 拿到的一般为YUV 详见 kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
        OSType type  = CVPixelBufferGetPixelFormatType(imageBuffer);
        if (type == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange)
        {
            //锁一下
            CVPixelBufferLockBaseAddress(imageBuffer, 0);
            //注：autoreleasepool 让内存快速回收
            @autoreleasepool {
                //CVPixelBufferRef 到 RGB UIImage 过程
                CIImage* ciImage = [CIImage imageWithCVPixelBuffer : imageBuffer];
                CIContext* context = [CIContext contextWithOptions : @{kCIContextUseSoftwareRenderer : @(YES)}];
                CGRect rect = CGRectMake(0, 0, CVPixelBufferGetWidth(imageBuffer), CVPixelBufferGetHeight(imageBuffer));
                CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
                CGImageRef videoImage = [context createCGImage : ciImage fromRect : rect];
                UIImage* image = [UIImage imageWithCGImage : videoImage];
                CGImageRelease(videoImage);
                videoImage = nil;
                ciImage = nil;
                
                //UIImage 转 data
                CFDataRef pixelData = CGDataProviderCopyData(CGImageGetDataProvider(image.CGImage));
                const uint8_t* data = CFDataGetBytePtr(pixelData);
                int width1 = (int)image.size.width;
                int height1 = (int)image.size.height;
                NSLog(@"rptest ----- width1:%d height1:%d", width1, height1);
                
                //赋值
                if (ct.length != width1 * height1 * 4)
                {
                    delete(ct.data);
                    ct.length = width1 * height1 * 4;
                    ct.data = new uint8[ct.length];
                }
                UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 5 %d!"), ct.length);
                uint8* ptr = (uint8*)data;
                
                //拷贝一份
                FMemory::Memcpy(ct.data,ptr,ct.length);
                CFRelease(pixelData);
                data = NULL;
                ptr = NULL;
                
                UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 6 !"));
                if (ct.length > 0)
                {
                    //ios gpu image direction is right.
                    ct.direction = FImageDirection::RIGHT;
                    result = true;
                    //测试用 - 保存为raw文件
//                    FString path = SpaceTargetDefinition::persistentPath() + FString("img.raw");
//                    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
//                    IFileHandle* FileHandle = PlatformFile.OpenWrite(*path);
//                    if (FileHandle)
//                    {
//                        FileHandle->Write(ct.data, ct.length);
//                        delete FileHandle;
//                    }
                }
                
                UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 7 !"));
            }

        }
        else
        {
            UE_LOG(LogSpaceTarget, Log, TEXT("Unsupported image buffer type"));
        }
        
      
#endif
    }
 
    //PF_R8G8B8A8
//    UARTexture* arTexture = UARBlueprintLibrary::GetARTexture(EARTextureType::CameraImage);
//    if (arTexture && arTexture->Resource && arTexture->Resource->TextureRHI)
//    {
//
//        UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 1 !"));
//        int32 width;
//        int32 height;
//
//        FTextureResource* TextureResource = arTexture->Resource;
//        ENQUEUE_RENDER_COMMAND(GetCameraTexture)(
//        [TextureResource,this,&width,&height,&isrunning](FRHICommandListImmediate& RHICmdList) //mutable
//        {
//            UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 2 ! isrunning: %s !"), *(isrunning ? FString("true") : FString("false")));
//            FTexture2DRHIRef Texture2D = TextureResource->TextureRHI->GetTexture2D();
//
//            FIntRect SourceRect(0, 0, Texture2D->GetSizeX(), Texture2D->GetSizeY());
//
//            width = SourceRect.Width();
//            height = SourceRect.Height();
//
//            FReadSurfaceDataFlags ReadDataFlags;
//            ReadDataFlags.SetLinearToGamma(false);
//
//            if (RawPixels.Num() != width * height)
//            {
//                RawPixels.SetNum(width * height);
//            }
//
//            RHICmdList.ReadSurfaceData(Texture2D, SourceRect, RawPixels, ReadDataFlags);
//
//            UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 3 ! %s"), *SourceRect.Size().ToString());
//
//            isrunning = false;
//        }
//        );
//
//        //cause ENQUEUE_RENDER_COMMAND is running on render thread,we don't wanna waste its time
//        while (isrunning)
//        {
//            FPlatformProcess::Sleep(0);
//        }
//        UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 4 %d!"), ct.length);
//        if (ct.length != RawPixels.Num() * 4)
//        {
//            delete(ct.data);
//            ct.length = RawPixels.Num() * 4;
//            ct.data = new uint8[ct.length];
//        }
//        UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 5 %d!"), ct.length);
//        uint8* ptr = ct.data;
//        //1.flip horizontal   2.clockwise rotate 90 degree
//        //for (int x = 0; x < width; x++)
//        //{
//        //    for (int y = 0; y < height; y++)
//        //    {
//        //        FColor color = RawPixels[y * width + x];
//        //        int curr = (y + x * height) * 4;
//        //        *(ptr + curr + 0) = color.R;
//        //        *(ptr + curr + 1) = color.G;
//        //        *(ptr + curr + 2) = color.B;
//        //        *(ptr + curr + 3) = 255;
//        //    }
//        //}
//        //.aniclockwise rotate 90 degree
//        for (int x = 0; x < width; x++)
//        {
//            for (int y = 0; y < height; y++)
//            {
//                FColor color = RawPixels[y * width + (width - x - 1)];
//                int curr =(y + x * height) * 4;
//                *(ptr + curr + 0) = color.R;
//                *(ptr + curr + 1) = color.G;
//                *(ptr + curr + 2) = color.B;
//                *(ptr + curr + 3) = 255;
//            }
//        }
//        UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 6 !"));
//        if (ct.length > 0)
//        {
//            //ios gpu image direction is right.
//            ct.direction = FImageDirection::RIGHT;
//            result = true;
//            //FString path = SpaceTargetDefinition::persistentPath() + FString("img.raw");
//            //IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
//            //IFileHandle* FileHandle = PlatformFile.OpenWrite(*path);
//            //if (FileHandle)
//            //{
//            //    FileHandle->Write(ct.data, ct.length);
//            //    delete FileHandle;
//            //}
//        }
//        UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 7 !"));
//    }
    return result;
}
void AARKitSlamGenerator::SaveFile(FString& path, const uint8* data, int32 len)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	UE_LOG(LogTemp, Warning, TEXT("save path:%s"), *path);
	IFileHandle* FileHandle = PlatformFile.OpenWrite(*path);
	if (FileHandle)
	{
		FileHandle->Write(data, len);
		delete FileHandle;
	}
}
