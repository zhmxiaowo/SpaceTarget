#include "ARKitSlamGenerator.h"
#include <ARBlueprintLibrary.h>
#include "SpaceTargetModule.h"

#include "ARBlueprintLibrary.h"
#include "SPARBaseAsyncTaskBlueprintProxy.h"
#if PLATFORM_IOS
#import <ARKit/ARKit.h>
//#include "AppleARKitSessionDelegate.h"
//#include "ARKitCoachingOverlay.h"
#import <Accelerate/Accelerate.h>
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
		//UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 2 ! ci.resolution: %s !"), *(ci.resolution.ToString()));
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
			//UE_LOG(LogSpaceTarget, Log, TEXT("cam_pos : %s  cam_qua : %s"), *(cam_pos.ToString()), *(cam_qua.ToString()));

			//unreal will change it automaticly.but we need the real rotation relative to camera image.
			switch (FPlatformMisc::GetDeviceOrientation())
			{
			case EDeviceScreenOrientation::Portrait:
				break;
			case EDeviceScreenOrientation::LandscapeLeft:
				cam_qua = cam_qua * FRotator(0, 0, 90).Quaternion();
				break;
			case EDeviceScreenOrientation::LandscapeRight:
				cam_qua = cam_qua * FRotator(0, 0, -90).Quaternion();
				break;
			case EDeviceScreenOrientation::PortraitUpsideDown:
				cam_qua = cam_qua * FRotator(0, 0, 180).Quaternion();//need test.
				break;
			default:
				break;
			}

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

TWeakPtr<FARSupportInterface, ESPMode::ThreadSafe> arSystem;
#if PLATFORM_IOS
ARFrame* nativeFrame;
#endif
bool AARKitSlamGenerator::GetCameraTexture(FCameraTexture& ct)
{
    bool result = false;
    volatile bool isrunning = true;

    if(arSystem == nullptr){
        arSystem = USPARBaseAsyncTaskBlueprintProxy::GetARSystemPublic();
    }
    
    if(arSystem == nullptr){
        return result;
    }
    
    if (arSystem.IsValid())
    {
#if PLATFORM_IOS
        
        nativeFrame = (ARFrame*)(arSystem.Pin()->GetGameThreadARFrameRawPointer());
        
        CVPixelBufferRef pixelBuffer = nativeFrame.capturedImage;

        //判断buffer的类型， YUV的还是 RGB的。
        //iOS ARKit 拿到的一般为YUV 详见 kCVPixelFormatType_420YpCbCr8BiPlanarFullRange
        OSType type  = CVPixelBufferGetPixelFormatType(pixelBuffer);

        if (type == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange)
        {
            
            CVPixelBufferLockBaseAddress(pixelBuffer, 0);
            //图像宽度（像素）
            size_t pixelWidth = CVPixelBufferGetWidth(pixelBuffer);
            //图像高度（像素）
            size_t pixelHeight = CVPixelBufferGetHeight(pixelBuffer);
            
            NSLog(@"picture width:%zu  height:%zu", pixelWidth, pixelHeight);
            
            void *yData = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
            size_t yHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 0);
            size_t yWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 0);
            size_t yPitch = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 0);
            
            vImage_Buffer yImage = {yData, yHeight, yWidth, yPitch};
            CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
            
            
            CVPixelBufferLockBaseAddress(pixelBuffer, 0);
            void *cDate = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
            size_t cHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 1);
            size_t cWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 1);
            size_t cBytesPerRow = CVPixelBufferGetBytesPerRowOfPlane(pixelBuffer, 1);

            vImage_Buffer cImage = {cDate, cHeight, cWidth, cBytesPerRow};
            CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
            
            
            CVPixelBufferRef outRef =NULL;
            size_t aRgbPitch = yWidth *  4;

            //malloc
            int width1 = (int)pixelWidth;
            int height1 = (int)pixelHeight;
        NSLog(@"rptest -- malloc ct len size: %d", width1 * height1 * 4);
            //赋值
            if (ct.length != width1 * height1 * 4)
            {
                NSLog(@"rptest -- run here ??? malloc ct???");
                delete(ct.data);
                ct.length = width1 * height1 * 4;
                ct.data = new uint8[ct.length];
            }
            


            void *aRgbBuffer = ct.data;
/*
            if(aRgbBuffer == nullptr){
                aRgbBuffer = malloc(yHeight *aRgbPitch);
            }else{
                memset(aRgbBuffer, 0, yHeight *aRgbPitch);
            }
*/
            //NSLog(@"rptest -- malloc size: %d", (int)(yHeight *aRgbPitch));
            
            CVPixelBufferCreateWithBytes(NULL, yWidth, yHeight, kCVPixelFormatType_32BGRA, aRgbBuffer, aRgbPitch, nil, nil, nil, &outRef);
//            CVPixelBufferCreateWithBytes(NULL, yWidth, yHeight, kCVPixelFormatType_32ARGB, aRgbBuffer, aRgbPitch, nil, nil, nil, &outRef);
            
            CVPixelBufferLockBaseAddress(outRef, 0);
            void *oDate = CVPixelBufferGetBaseAddress(outRef);
            size_t oHeight = CVPixelBufferGetHeight(outRef);
            size_t oWidth = CVPixelBufferGetWidth(outRef);
            size_t oBytesPerRow = CVPixelBufferGetBytesPerRow(outRef);
            vImage_Buffer oImage = {oDate, oHeight, oWidth, oBytesPerRow};
            CVPixelBufferUnlockBaseAddress(outRef, 0);
            
            vImage_YpCbCrPixelRange pixelRange = {0, 128, 255, 255, 255, 1, 255, 0};
            
            vImage_YpCbCrToARGB infoYpCbCrToARGB = {};
            vImage_Error error = vImageConvert_YpCbCrToARGB_GenerateConversion(kvImage_YpCbCrToARGBMatrix_ITU_R_709_2, &pixelRange, &infoYpCbCrToARGB, kvImage420Yp8_CbCr8, kvImageARGB8888, kvImageNoFlags);
            if (error != kvImageCVImageFormat_NoError) {
                NSLog(@"rptest ---- error111!!! %zd", error);
            }
            
            vImage_Error ToError = vImageConvert_420Yp8_CbCr8ToARGB8888(&yImage, &cImage, &oImage, &infoYpCbCrToARGB, nil, 255, kvImageNoFlags);
            if (ToError != kvImageCVImageFormat_NoError) {
                NSLog(@"rptest ---- error222!!! %zd", ToError);
            }
            uint8_t permuteMap[4] = { 3, 2, 1, 0 };
            vImagePermuteChannels_ARGB8888(&oImage, &oImage, permuteMap, 0);
            


            uint8* ptr = ct.data;
//            uint8* ptr = (uint8*)aRgbBuffer;
            int r,g,b,a;
            for (int x = 0; x < ct.length; x+=4)
            {
                r = *(ptr + x + 2);
                g = *(ptr + x + 1);
                b = *(ptr + x + 0);
                a = *(ptr + x + 3);
                
                *(ptr + x + 0) = r;
                *(ptr + x + 1) = g;
                *(ptr + x + 2) = b;
                *(ptr + x + 3) = a;
            }
        

//            aRgbBuffer = NULL;
//            free(aRgbBuffer);
            //NSLog(@"rptest ciimage end-----");

            if (ct.length > 0)
            {
                //ios gpu image direction is right.
                ct.direction = FImageDirection::LEFT;
                result = true;
                //测试用 - 保存为raw文件
                //FString path = SpaceTargetDefinition::persistentPath() + FString("img.raw");
                //IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
                //IFileHandle* FileHandle = PlatformFile.OpenWrite(*path);
                //if (FileHandle)
                //{
                //    FileHandle->Write(ct.data, ct.length);
                //    delete FileHandle;
                //}
            }
 

            //UE_LOG(LogSpaceTarget, Log, TEXT("arTexture is 7 !"));
        }
        else
        {
            //UE_LOG(LogSpaceTarget, Log, TEXT("Unsupported image buffer type"));
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
