// Fill out your copyright notice in the Description page of Project Settings.
#include "SpaceTargetSubsystem.h"
#include "SlamGenerator.h"

#if PLATFORM_ANDROID
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#endif
#include <JsonObjectConverter.h>
#include "AndroidUtility.h"
#include "iOSUtility.h"
#include "MathUtil.h"
#include "Containers/Ticker.h"
#include "HAL/PlatformFileManager.h"
#include "Async/Async.h"

bool USpaceTargetSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (Outer->GetWorld()->IsGameWorld())
	{
		return true;
	}
	else
	{
		return false;
	}
}
void USpaceTargetSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogSpaceTarget, Warning, TEXT("USpaceTargetSubsystem has create"));

	FTickerDelegate tickDelegate = FTickerDelegate::CreateUObject(this, &USpaceTargetSubsystem::Tick);
	tickHandle = FTicker::GetCoreTicker().AddTicker(tickDelegate);
}

void USpaceTargetSubsystem::Deinitialize()
{
	FTicker::GetCoreTicker().RemoveTicker(tickHandle);
	UE_LOG(LogSpaceTarget, Warning, TEXT("USpaceTargetSubsystem has released"));
}

bool USpaceTargetSubsystem::Init(const ASpaceTargetTrackbleActor* actor,const TScriptInterface<ISlamGenerator>& platformSlam)
{
    currStatus = 0;
	camStatus = 0;
    _cTexture.data = NULL;
	isRelocate = false;
	blockTime = FDateTime::Now();

    stActor = const_cast<ASpaceTargetTrackbleActor*>(actor);
    stSlam = platformSlam;
    FString dataId = actor->dataBase;
	UE_LOG(LogSpaceTarget, Warning, TEXT("USpaceTargetSubsystem init data id :%s"), *dataId);

    bool callresult = false;

    IPlatformFile& ipf = FPlatformFileManager::Get().GetPlatformFile();
    SpaceTargetDefinition::Initialize(dataId);
    FString path = SpaceTargetDefinition::persistentPathInner();
	FString workpath = SpaceTargetDefinition::persistentPath();
    FString srcPath = SpaceTargetDefinition::dataAssetsPath()+ dataId +"/";
    if (ipf.DirectoryExists(*path))
    {
        //clean the directory
        ipf.DeleteDirectory(*path);
    }
    ipf.CreateDirectoryTree(*path);

    if (!ipf.FileExists(*(path + SpaceTargetDefinition::sfmbin)))
    {
        ipf.CopyFile(*(path + SpaceTargetDefinition::sfmbin), *(srcPath + SpaceTargetDefinition::sfmbin));
        ipf.CopyFile(*(path + SpaceTargetDefinition::sfmdesc), *(srcPath + SpaceTargetDefinition::sfmdesc));
        ipf.CopyFile(*(path + SpaceTargetDefinition::sfmfeat), *(srcPath + SpaceTargetDefinition::sfmfeat));
    }
	//TODO: ios xcode havn't automatically configure tool.so we put the tflite in desk to read.
#if PLATFORM_IOS
	if (!ipf.FileExists(*(workpath + SpaceTargetDefinition::enctflite)))
	{
		ipf.CopyFile(*(workpath + SpaceTargetDefinition::enctflite), *(SpaceTargetDefinition::dataAssetsPath() + SpaceTargetDefinition::enctflite));
	}
#endif
    UE_LOG(LogTemp,Log,TEXT("init data path : %s"),*workpath);
#if PLATFORM_ANDROID
    callresult = AndroidUtility::Init(workpath);
#elif PLATFORM_IOS
	callresult = iOSUtility::Init(workpath);
#else
    //pc
    callresult = true;
#endif
    
	return callresult;
}

bool USpaceTargetSubsystem::Start()
{
    needToTick = true;
    return true;
}

void USpaceTargetSubsystem::Stop()
{
    needToTick = false;
}

bool isFound = false;
bool USpaceTargetSubsystem::Tick(float Deltatime)
{
    if (needToTick && stActor && stSlam)
    {
        //UE_LOG(LogTemp, Warning, TEXT("subsystem 1"));
		if(isRelocate)
		{
			return true;
		}
		//@TODO: if enable this, cpu will back to 100%,otherwise 260%,cause view lag.close it will make tracking more fast.
//#if PLATFORM_IOS
//		if ((FDateTime::Now() - blockTime).GetTotalMilliseconds() < 1000)
//		{
//			return true;
//		}
//#endif
		isRelocate = true;
        FXRTrackingState xrState;
        stSlam->GetTrackingStatus(xrState);
        //UE_LOG(LogTemp, Warning, TEXT("subsystem 2"));
		if (xrState == FXRTrackingState::Tracking)
		{
			Async(EAsyncExecution::ThreadPool, [&]()
			{
				volatile bool bPose = stSlam->GetCameraPose(_cPose);
				volatile bool bIntrinsics = stSlam->GetCameraIntrinsics(_cIntrisics);
				volatile bool bTexture = stSlam->GetCameraTexture(_cTexture);
				//UE_LOG(LogTemp, Warning, TEXT("subsystem 3"));
				//UE_LOG(LogTemp, Warning, TEXT("bPose : %s bIntrinsics : %s  bTexture: %s  "), *(bPose ? FString("true") : FString("false")), *(bIntrinsics ? FString("true") : FString("false")), *(bTexture ? FString("true") : FString("false")));
				if (bPose && bIntrinsics && bTexture)
				{
					bool algorithomCall = false;
					//UE_LOG(LogTemp, Warning, TEXT("subsystem 4"));
					auto task = Async(EAsyncExecution::ThreadPool, [&]()
					{
						FixImageAixs(_cTexture.direction,this->aixs);
#if PLATFORM_ANDROID
						//because algorithom use m,unreal engine use cm.
						algorithomCall = AndroidUtility::Relocate((_cPose.position * 0.01f), _cPose.rotation, _cIntrisics.focal, _cIntrisics.pricipal, _cIntrisics.resolution, &_cTexture.data, _cTexture.length, currStatus,aixs.X,aixs.Y,aixs.Z, camStatus);
#elif PLATFORM_IOS
						algorithomCall = iOSUtility::Relocate((_cPose.position * 0.01f), _cPose.rotation, _cIntrisics.focal, _cIntrisics.pricipal, _cIntrisics.resolution, &_cTexture.data, _cTexture.length, currStatus,aixs.X,aixs.Y,aixs.Z, camStatus);
#else
						algorithomCall = true;
#endif
					});
					//it will break current thread.
					task.Wait();

					camStatus = 1;

					//UE_LOG(LogTemp, Warning, TEXT("subsystem 5"));
					//UE_LOG(LogTemp, Warning, TEXT("algorithomCall : %s"), *(algorithomCall ? FString("true") : FString("false")));
					FOutData newData;

					if (algorithomCall)
					{
						FString jsonData;
#if PLATFORM_ANDROID
						algorithomCall = AndroidUtility::GetResult(jsonData);
#elif PLATFORM_IOS
						algorithomCall = iOSUtility::GetResult(jsonData);
#else
						algorithomCall = true;
#endif 
						if (algorithomCall && !jsonData.IsEmpty())
						{
							if (FJsonObjectConverter::JsonObjectStringToUStruct(jsonData, &newData, 0, 0))
							{
								newData.poseOfAR.position *= 100;
								if(newData.NFA < minimunNFA)
								{
									algorithomCall = true;
								}
								else
								{
									algorithomCall = false;
								}
							}
							else
							{
								algorithomCall = false;
							}
						}
					}

					//mark the number of success or failure
					if (algorithomCall)
					{
						//success
						successCount++;
						if (successCount > maxSuccessCount)
						{
							failureCount = 0;
							successCount = maxSuccessCount;
							currStatus = 1;
						}
					}
					else
					{
						//failure
						failureCount++;
						if (failureCount > maxFailureCount)
						{
							successCount = 0;
							currStatus = 0;
							failureCount = maxFailureCount;
						}
					}

					volatile bool successOnMainThread = false;
					volatile bool adjustmentOnMaththread = false;

					//for running in the same frame,we move this action below.
					if (algorithomCall && !isFound)
					{
						successOnMainThread = true;
					}

					if (algorithomCall)
					{
						//比较两个模型的角度差
						FVector dir1 = newData.poseOfAR.rotation.Vector().GetSafeNormal();
						FVector dir2 = oldData.poseOfAR.rotation.Vector().GetSafeNormal();
						
						float angleBetween = FMathf::ACos(FVector::DotProduct(dir1,dir2)) * FMathf::RadToDeg;
						//float angleBetween = FVector::DotProduct(dir1, dir2);

						//比较两个模型的位置差
						float distanceBetween = (newData.poseOfAR.position - oldData.poseOfAR.position).Size();

						//比较两次算法精度差
						float NFABetween = oldData.NFA - newData.NFA;

						//-------------------------------------------------------------------------------------------------------
						//需要修改位置匹配模型
						if (angleBetween > 10 || distanceBetween > 100 || NFABetween > 50 )
						{
							adjustmentOnMaththread = true;
							oldData = newData;
						}
						//adjustmentOnMaththread = true;
						//UE_LOG(LogSpaceTarget,Log,TEXT("angleBetween:%f   distanceBetween:%f   NFABetween:%f"), angleBetween, distanceBetween, NFABetween);
					}

					//UE_LOG(LogTemp, Warning, TEXT("successOnMainThread: %s adjustmentOnMaththread: %s"), *(successOnMainThread ? FString("true") : FString("false")), *(adjustmentOnMaththread ? FString("true") : FString("false")));
					if (successOnMainThread || adjustmentOnMaththread)
					{
						AsyncTask(ENamedThreads::GameThread, [&, algorithomCall, newData]()
						{
							if (successOnMainThread)
							{
								if (successCount == maxSuccessCount)
								{
									isFound = true;
									if (onSpaceTargetFoundBP.IsBound())
									{
										onSpaceTargetFoundBP.Broadcast(newData);
									}
									//UE_LOG(LogTemp, Warning, TEXT("subsystem 6-2"));
									adjustmentOnMaththread = true;
								}
							}
							if (adjustmentOnMaththread)
							{
								//UE_LOG(LogTemp, Warning, TEXT("Relocate success !"));
								FTransform newPose;
								if (stActor->centerMode == FWorldCenterMode::Target)
								{
									//FTransform newTransform(newData.poseOfAR.rotation,newData.poseOfAR.position);

									stSlam->GetCameraPose(_cNewPose);
									//FTransform camNewTransf(_cNewPose.rotation, _cNewPose.position);

									//FTransform poseToTarget = newTransform.Inverse() * stActor->GetTransform();

									//FTransform camTransform = camNewTransf * poseToTarget;

									//stSlam->SetCameraPose(camTransform);

									FMatrix matrix_curr_cordinate = FQuatRotationTranslationMatrix::Make(newData.poseOfAR.rotation, newData.poseOfAR.position);

									FMatrix matrix_new_cordinate(stActor->GetActorForwardVector(), stActor->GetActorRightVector(), stActor->GetActorUpVector(), stActor->GetActorLocation());

									FMatrix Q = matrix_curr_cordinate.Inverse() * matrix_new_cordinate;

									FVector newPosition = Q.TransformPosition(_cNewPose.position);
									FQuat newRotation = Q.ToQuat() * _cNewPose.rotation;

									newPose.SetLocation(newPosition);
									newPose.SetRotation(newRotation);

									stSlam->SetCameraPose(newPose);
								}
								else
								{
									stActor->SetActorLocation(newData.poseOfAR.position);
									stActor->SetActorRotation(newData.poseOfAR.rotation);
									if (onSpaceTargetPoseChangeBP.IsBound())
									{
										onSpaceTargetPoseChangeBP.Broadcast(stActor, stActor->GetTransform());
									}
								}
								//UE_LOG(LogTemp, Warning, TEXT("subsystem 6-1"));
							}
							isRelocate = false;
						});
					}
					else
					{
						isRelocate = false;
					}
				}
				else
				{
					isRelocate = false;
				}
//#if PLATFORM_IOS
//				blockTime = FDateTime::Now();
//#endif
			});

        }
        else
        {
            //tracking unstable.
			if (isFound)
			{
				isFound = false;
				AsyncTask(ENamedThreads::GameThread, [&]()
				{
					if (onSpacetTargetLostBP.IsBound())
					{
						onSpacetTargetLostBP.Broadcast();
					}
					isRelocate = false;
				});
			}
			else
			{
				isRelocate = false;
			}
			camStatus = 0;
        }
    }

	return true;
}

void USpaceTargetSubsystem::FixImageAixs(FImageDirection dir, ImageAixs& tempaix)
{
	switch (dir)
	{
	case FImageDirection::LEFT:
		tempaix.X = FString("-z");
		tempaix.Y = FString("-y");
		tempaix.Z = FString("x");
		break;
	case FImageDirection::RIGHT:
		tempaix.X = FString("z");
		tempaix.Y = FString("y");
		tempaix.Z = FString("x");
		break;
	case FImageDirection::UP:
		tempaix.X = FString("y");
		tempaix.Y = FString("-z");
		tempaix.Z = FString("x");
		break;
	case FImageDirection::DOWN:
		tempaix.X = FString("-y");
		tempaix.Y = FString("z");
		tempaix.Z = FString("x");
		break;
	}
}
